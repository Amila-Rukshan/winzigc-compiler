#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/visitor.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

void IntegerExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void BooleanExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void CharacterExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void CallExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void IdentifierExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void AssignmentExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void IfExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void ForExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void RepeatUntilExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void WhileExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void CaseExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void ReturnExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void SwapExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void BinaryExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

void UnaryExpression::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
