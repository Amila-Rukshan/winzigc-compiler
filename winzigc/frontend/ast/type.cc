#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void IntegerType::accept(Visitor& visitor) const { visitor.visit(*this); }

void BooleanType::accept(Visitor& visitor) const { visitor.visit(*this); }

void CharacterType::accept(Visitor& visitor) const { visitor.visit(*this); }

void UserType::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
