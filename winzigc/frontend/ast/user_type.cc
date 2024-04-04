#include "winzigc/frontend/ast/user_type.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void GlobalUserTypeDef::accept(Visitor& visitor) const { visitor.visit(*this); }

void LocalUserTypeDef::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
