#include "winzigc/frontend/ast/function.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void Function::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
