#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

llvm::Type* IntegerType::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Type* BooleanType::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Type* CharacterType::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Type* UserType::accept(Visitor& visitor) const { return visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
