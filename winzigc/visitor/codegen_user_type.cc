#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalUserTypeDef& expression) {
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::LocalUserTypeDef& expression) {
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
