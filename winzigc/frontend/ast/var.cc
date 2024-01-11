#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

llvm::Value* GlobalVariable::accept(Visitor& visitor) const { return visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
