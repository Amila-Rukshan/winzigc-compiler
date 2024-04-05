#include "winzigc/frontend/ast/program.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void Program::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
