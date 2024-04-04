#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void GlobalVariable::accept(Visitor& visitor) const { visitor.visit(*this); }

void LocalVariable::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
