#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/visitor.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

llvm::Value* IntegerExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* BooleanExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* CharacterExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* CallExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* IdentifierExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* AssignmentExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* IfExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* ForExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* RepeatUntilExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* WhileExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* CaseExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* ReturnExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* SwapExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* BinaryExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

llvm::Value* UnaryExpression::accept(Visitor& visitor) const { return visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
