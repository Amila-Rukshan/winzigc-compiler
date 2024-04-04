#pragma once

#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/user_type.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/function.h"
#include "winzigc/common/pure.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor {
public:
  virtual ~Visitor() = default;

  virtual void visit(const IntegerExpression& expression) PURE;
  virtual void visit(const BooleanExpression& expression) PURE;
  virtual void visit(const CharacterExpression& expression) PURE;

  virtual void visit(const CallExpression& expression) PURE;
  virtual void visit(const IdentifierExpression& expression) PURE;
  virtual void visit(const AssignmentExpression& expression) PURE;
  virtual void visit(const SwapExpression& expression) PURE;
  virtual void visit(const IfExpression& expression) PURE;
  virtual void visit(const ForExpression& expression) PURE;
  virtual void visit(const RepeatUntilExpression& expression) PURE;
  virtual void visit(const WhileExpression& expression) PURE;
  virtual void visit(const CaseExpression& expression) PURE;
  virtual void visit(const ReturnExpression& expression) PURE;
  virtual void visit(const BinaryExpression& expression) PURE;
  virtual void visit(const UnaryExpression& expression) PURE;

  virtual void visit(const GlobalVariable& expression) PURE;
  virtual void visit(const LocalVariable& expression) PURE;

  virtual void visit(const GlobalUserTypeDef& expression) PURE;
  virtual void visit(const LocalUserTypeDef& expression) PURE;

  virtual void visit(const IntegerType& expression) PURE;
  virtual void visit(const BooleanType& expression) PURE;
  virtual void visit(const CharacterType& expression) PURE;
  virtual void visit(const UserType& expression) PURE;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
