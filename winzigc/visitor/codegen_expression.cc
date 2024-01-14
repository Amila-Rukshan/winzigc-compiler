#include <iostream>
#include <string>

#include "winzigc/visitor/codegen_visitor.h"

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
  llvm::Function* callee_func;
  if (expression.get_name() == "output") {
    callee_func = module->getFunction("printf");
  } else {
    callee_func = module->getFunction(expression.get_name());
  }
  if (!callee_func) {
    std::cerr << "Unknown function referenced" << std::endl;
    return nullptr;
  }
  // TODO: check argument types and count
  std::vector<llvm::Value*> args;
  if (expression.get_name() == "output") {
    llvm::Value* format_str = builder->CreateGlobalStringPtr("%d\n");
    args.push_back(format_str);
  }
  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      std::cerr << "Unknown argument" << std::endl;
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
    std::cerr << "Unknown variable name" << std::endl;
    return nullptr;
  }
  return builder->CreateLoad(var, expression.get_name());
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  llvm::GlobalVariable* var = module->getNamedGlobal(expression.get_name().get_name());
  if (!var) {
    std::cerr << "Unknown variable name" << std::endl;
    return nullptr;
  }
  llvm::Value* value = expression.get_expression().accept(*this);
  builder->CreateStore(value, var);
  return value;
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
  default:
    std::cerr << "Unknown binary operation" << std::endl;
    return nullptr;
  }
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
