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

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::BooleanExpression& expression) {
  return expression.get_bool() ? llvm::ConstantInt::getTrue(*context)
                               : llvm::ConstantInt::getFalse(*context);
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::CharacterExpression& expression) {
  return llvm::ConstantInt::getSigned(llvm::Type::getInt8Ty(*context), expression.get_character());
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

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::IdentifierExpression& expression) {
  llvm::Value* var = lookup_variable(expression.get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return nullptr;
  }
  return builder->CreateLoad(var, expression.get_name());
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  llvm::Value* var = lookup_variable(expression.get_name().get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return nullptr;
  }
  llvm::Value* value = expression.get_expression().accept(*this);
  builder->CreateStore(value, var);
  return value;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::SwapExpression& expression) {
  llvm::Value* var1 = lookup_variable(expression.get_lhs().get_name());
  llvm::Value* var2 = lookup_variable(expression.get_rhs().get_name());
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
  bool add_merge_block = false;

  // condition block instructions
  builder->CreateCondBr(cond, then_block, else_block);

  // then block instructions
  builder->SetInsertPoint(then_block);
  for (const auto& statement : expression.get_then_statement()) {
    statement->accept(*this);
  }
  // final block branched from then block does not end with a terminator
  if (!builder->GetInsertBlock()->getTerminator()) {
    add_merge_block = true;
    builder->CreateBr(merge_block);
  }

  // else block instructions
  parent_function->getBasicBlockList().push_back(else_block);
  builder->SetInsertPoint(else_block);
  for (const auto& statement : expression.get_else_statement()) {
    statement->accept(*this);
  }
  // final block branched from else block does not end with a terminator
  if (!builder->GetInsertBlock()->getTerminator()) {
    add_merge_block = true;
    builder->CreateBr(merge_block);
  }

  // we need a merge block only when alleast one branch is not going to the fucntion exit block
  if (add_merge_block) {
    parent_function->getBasicBlockList().push_back(merge_block);
    builder->SetInsertPoint(merge_block);
  }

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::ForExpression& expression) {
  llvm::Function* function = builder->GetInsertBlock()->getParent();

  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "for_cond", function);
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "for_body");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "for_exit");

  expression.get_start_assignment().accept(*this);
  builder->CreateBr(cond_block);

  builder->SetInsertPoint(cond_block);
  llvm::Value* cond = expression.get_condition().accept(*this);
  builder->CreateCondBr(cond, body_block, exit_block);

  function->getBasicBlockList().push_back(body_block);
  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
  expression.get_end_assignment().accept(*this);
  builder->CreateBr(cond_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::RepeatUntilExpression& expression) {
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "repeat_body", function);
  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "repeat_cond");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "repeat_exit");

  builder->CreateBr(body_block);

  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }

  builder->CreateBr(cond_block);
  builder->SetInsertPoint(cond_block);
  function->getBasicBlockList().push_back(cond_block);
  llvm::Value* cond = expression.get_condition().accept(*this);
  builder->CreateCondBr(cond, exit_block, body_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::WhileExpression& expression) {
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "while_cond", function);
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "while_body");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "while_exit");

  builder->CreateBr(cond_block);
  builder->SetInsertPoint(cond_block);

  llvm::Value* cond = expression.get_condition().accept(*this);
  builder->CreateCondBr(cond, body_block, exit_block);

  function->getBasicBlockList().push_back(body_block);
  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
  builder->CreateBr(cond_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::CaseExpression& expression) {
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "case_exit");
  bool add_exit_block = false;

  llvm::Value* switch_val = expression.get_expression().accept(*this);
  llvm::SwitchInst* switch_inst =
      builder->CreateSwitch(switch_val, exit_block, expression.get_cases().size());

  for (size_t i = 0; i < expression.get_cases().size(); i++) {
    const Frontend::AST::CaseClause& case_clause = expression.get_cases()[i];
    std::string blockName = "case_" + std::to_string(i);
    llvm::BasicBlock* case_block = llvm::BasicBlock::Create(*context, blockName, function);

    const auto& case_value = case_clause.first;
    Frontend::AST::Expression* value_expr = nullptr;
    if (std::holds_alternative<std::unique_ptr<Frontend::AST::Expression>>(case_value)) {
      value_expr = std::get<std::unique_ptr<Frontend::AST::Expression>>(case_value).get();
    } else {
      LOG(ERROR) << "Not implemented! 'range case value'";
    }
    llvm::Value* llvm_case_value = value_expr->accept(*this);
    llvm::ConstantInt* const_int = llvm::dyn_cast<llvm::ConstantInt>(llvm_case_value);
    switch_inst->addCase(const_int, case_block);

    builder->SetInsertPoint(case_block);
    for (const auto& statement : case_clause.second) {
      statement->accept(*this);
    }

    if (!builder->GetInsertBlock()->getTerminator()) {
      add_exit_block = true;
      builder->CreateBr(exit_block);
    }
  }

  if (add_exit_block) {
    function->getBasicBlockList().push_back(exit_block);
    builder->SetInsertPoint(exit_block);
  }

  return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::ReturnExpression& expression) {
  llvm::Function* parent_function = builder->GetInsertBlock()->getParent();
  llvm::Value* return_var = lookup_variable(parent_function->getName().str());
  if (!return_var) {
    LOG(ERROR) << "Unknown return variable name";
    return nullptr;
  }
  llvm::Value* return_val = expression.get_expression().accept(*this);
  builder->CreateStore(return_val, return_var);
  builder->CreateBr(function_exit_block);
  return nullptr;
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

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::UnaryExpression& expression) {
  llvm::Value* operand = expression.get_expression().accept(*this);
  switch (expression.get_op()) {
  case Frontend::AST::UnaryOperation::kMinus:
    return builder->CreateNeg(operand, "negtmp");
  case Frontend::AST::UnaryOperation::kPlus:
    return operand;
  case Frontend::AST::UnaryOperation::kNot:
    return builder->CreateNot(operand, "nottmp");
  case Frontend::AST::UnaryOperation::kSucc:
    return builder->CreateAdd(operand, llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 1), "addtmp");
  case Frontend::AST::UnaryOperation::kPred:
    return builder->CreateSub(operand, llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 1), "subtmp");
  default:
    LOG(ERROR) << "Unknown unary operation";
    return nullptr;
  }
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
