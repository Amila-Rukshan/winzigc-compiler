#include "winzigc/frontend/ast/user_type.h"
#include "winzigc/frontend/ast/visitor.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

llvm::Value* GlobalUserTypeDef::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* LocalUserTypeDef::accept(Visitor& visitor) const { return visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
