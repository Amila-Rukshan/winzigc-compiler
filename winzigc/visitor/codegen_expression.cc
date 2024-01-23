#include <iostream>
#include <string>

#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::IntegerExpression& expression) {
  return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), expression.get_value());
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::VariableExpression& expression) {
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::CallExpression& expression) {
  if (expression.get_name() == "output") {
    return codegen_output_call(expression);
  } else if (expression.get_name() == "read") {
    return codegen_read_call(expression);
  }

  llvm::Function* callee_func = module->getFunction(expression.get_name());
  if (!callee_func) {
    LOG(ERROR) << "Unknown function referenced";
    return nullptr;
  }

  std::vector<llvm::Value*> args;
  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return nullptr;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isPointerTy()) {
      arg_val = builder->CreateLoad(arg_val);
    }
    args.push_back(arg_val);
  }
  return builder->CreateCall(callee_func, args);
}

llvm::Value* CodeGenVisitor::codegen_read_call(const Frontend::AST::CallExpression& expression) {
  llvm::Function* callee_func = module->getFunction("scanf");
  if (!callee_func) {
    LOG(ERROR) << "scanf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;
  args.push_back(builder->CreateGlobalStringPtr("%d"));

  for (const auto& arg : expression.get_arguments()) {
    if (const Frontend::AST::IdentifierExpression* var_identifier =
            dynamic_cast<const Frontend::AST::IdentifierExpression*>(arg.get())) {
      llvm::GlobalVariable* i_global =
          global_variables[llvm::StringRef(var_identifier->get_name())];
      if (!i_global) {
        LOG(ERROR) << "Unknown global variable name";
        return nullptr;
      }
      llvm::ConstantInt* zero = llvm::ConstantInt::getSigned(llvm::Type::getInt64Ty(*context), 0);
      llvm::Value* i_ptr = builder->CreateInBoundsGEP(i_global, zero);
      args.push_back(i_ptr);
    } else {
      LOG(ERROR) << "'read' called with non global variable";
    }
  }
  return builder->CreateCall(callee_func, args);
}

llvm::Value* CodeGenVisitor::codegen_output_call(const Frontend::AST::CallExpression& expression) {
  llvm::Function* callee_func = module->getFunction("printf");
  if (!callee_func) {
    LOG(ERROR) << "printf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;
  std::string format_str = "%d";
  for (int i = 1; i < expression.get_arguments().size(); ++i) {
    format_str += " %d";
  }
  format_str += "\n";
  args.push_back(builder->CreateGlobalStringPtr(format_str));

  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return nullptr;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isPointerTy()) {
      arg_val = builder->CreateLoad(arg_val);
    }
    args.push_back(arg_val);
  }
  return builder->CreateCall(callee_func, args);
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::IdentifierExpression& expression) {
  // get value for the global variable
  // TODO: check if the variable is declared or it is local
  llvm::GlobalVariable* var = module->getNamedGlobal(expression.get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return nullptr;
  }
  return builder->CreateLoad(var, expression.get_name());
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  llvm::GlobalVariable* var = module->getNamedGlobal(expression.get_name().get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return nullptr;
  }
  llvm::Value* value = expression.get_expression().accept(*this);
  builder->CreateStore(value, var);
  return value;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::SwapExpression& expression) {
  llvm::GlobalVariable* var1 = module->getNamedGlobal(expression.get_lhs().get_name());
  llvm::GlobalVariable* var2 = module->getNamedGlobal(expression.get_rhs().get_name());
  if (!var1 || !var2) {
    LOG(ERROR) << "Unknown variable name";
    return nullptr;
  }
  llvm::Value* value1 = builder->CreateLoad(var1);
  llvm::Value* value2 = builder->CreateLoad(var2);
  builder->CreateStore(value2, var1);
  builder->CreateStore(value1, var2);
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::IfExpression& expression) {
  llvm::Value* cond = expression.get_condition().accept(*this);
  if (!cond) {
    LOG(ERROR) << "Unknown condition";
    return nullptr;
  }

  llvm::Function* parent_function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* then_block = llvm::BasicBlock::Create(*context, "then", parent_function);
  llvm::BasicBlock* else_block = llvm::BasicBlock::Create(*context, "else");
  llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(*context, "ifcont");

  builder->CreateCondBr(cond, then_block, else_block);

  builder->SetInsertPoint(then_block);

  llvm::Value* then_val = nullptr;
  for (const auto& statement : expression.get_then_statement()) {
    then_val = statement->accept(*this);
  }

  builder->CreateBr(merge_block);
  then_block = builder->GetInsertBlock();

  parent_function->getBasicBlockList().push_back(else_block);
  builder->SetInsertPoint(else_block);

  llvm::Value* else_val = nullptr;
  for (const auto& statement : expression.get_else_statement()) {
    else_val = statement->accept(*this);
  }

  builder->CreateBr(merge_block);
  else_block = builder->GetInsertBlock();

  parent_function->getBasicBlockList().push_back(merge_block);
  builder->SetInsertPoint(merge_block);

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::BinaryExpression& expression) {
  llvm::Value* lhs = expression.get_lhs().accept(*this);
  llvm::Value* rhs = expression.get_rhs().accept(*this);

  switch (expression.get_op()) {
  case Frontend::AST::BinaryOperation::kAdd:
    return builder->CreateAdd(lhs, rhs, "addtmp");
  case Frontend::AST::BinaryOperation::kSubtract:
    return builder->CreateSub(lhs, rhs, "subtmp");
  case Frontend::AST::BinaryOperation::kMultiply:
    return builder->CreateMul(lhs, rhs, "multmp");
  case Frontend::AST::BinaryOperation::kDivide:
    return builder->CreateSDiv(lhs, rhs, "divtmp");
  case Frontend::AST::BinaryOperation::kModulo:
    return builder->CreateSRem(lhs, rhs, "modtmp");
  case Frontend::AST::BinaryOperation::kLessThan:
    return builder->CreateICmpSLT(lhs, rhs, "lttmp");
  case Frontend::AST::BinaryOperation::kLessThanOrEqual:
    return builder->CreateICmpSLE(lhs, rhs, "letmp");
  case Frontend::AST::BinaryOperation::kGreaterThan:
    return builder->CreateICmpSGT(lhs, rhs, "gttmp");
  case Frontend::AST::BinaryOperation::kGreaterThanOrEqual:
    return builder->CreateICmpSGE(lhs, rhs, "getmp");
  case Frontend::AST::BinaryOperation::kEqual:
    return builder->CreateICmpEQ(lhs, rhs, "eqtmp");
  case Frontend::AST::BinaryOperation::kNotEqual:
    return builder->CreateICmpNE(lhs, rhs, "netmp");
  case Frontend::AST::BinaryOperation::kAnd:
    return builder->CreateAnd(lhs, rhs, "andtmp");
  case Frontend::AST::BinaryOperation::kOr:
    return builder->CreateOr(lhs, rhs, "ortmp");
  default:
    LOG(ERROR) << "Unknown binary operation";
    return nullptr;
  }
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
