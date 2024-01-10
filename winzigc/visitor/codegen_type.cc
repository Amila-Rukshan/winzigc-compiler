#include "winzigc/visitor/codegen_visitor.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"

namespace WinZigC {
namespace Visitor {

llvm::Type* CodeGenVisitor::visit(const Frontend::AST::IntegerType expression) {
  return llvm::Type::getInt32Ty(*context);
}

llvm::Type* CodeGenVisitor::visit(const Frontend::AST::BooleanType expression) {
  return llvm::Type::getInt1Ty(*context);
}

} // namespace Visitor
} // namespace WinZigC
