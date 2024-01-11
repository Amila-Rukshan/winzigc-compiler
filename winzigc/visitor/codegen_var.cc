#include "winzigc/visitor/codegen_visitor.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Visitor {

 llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalVariable& expression) {
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
