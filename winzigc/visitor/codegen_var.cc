#include "winzigc/visitor/codegen_visitor.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalVariable& expression) {
  module->getOrInsertGlobal(expression.get_name(), expression.get_type().accept(*this));
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
