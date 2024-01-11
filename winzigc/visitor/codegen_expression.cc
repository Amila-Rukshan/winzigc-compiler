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
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::IdentifierExpression& expression) {
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::BinaryExpression& expression) {
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
