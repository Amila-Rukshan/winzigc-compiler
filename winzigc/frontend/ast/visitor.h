#pragma once

#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/type.h"
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

  virtual llvm::Value* visit(const IntegerExpression& expression) PURE;
  virtual llvm::Value* visit(const BooleanExpression& expression) PURE;
  virtual llvm::Value* visit(const CallExpression& expression) PURE;
  virtual llvm::Value* visit(const IdentifierExpression& expression) PURE;
  virtual llvm::Value* visit(const AssignmentExpression& expression) PURE;
  virtual llvm::Value* visit(const SwapExpression& expression) PURE;
  virtual llvm::Value* visit(const IfExpression& expression) PURE;
  virtual llvm::Value* visit(const ForExpression& expression) PURE;
  virtual llvm::Value* visit(const RepeatUntilExpression& expression) PURE;
  virtual llvm::Value* visit(const WhileExpression& expression) PURE;
  virtual llvm::Value* visit(const ReturnExpression& expression) PURE;
  virtual llvm::Value* visit(const BinaryExpression& expression) PURE;
  virtual llvm::Value* visit(const UnaryExpression& expression) PURE;

  virtual llvm::Value* visit(const GlobalVariable& expression) PURE;
  virtual llvm::Value* visit(const LocalVariable& expression) PURE;

  virtual llvm::Type* visit(const IntegerType& expression) PURE;
  virtual llvm::Type* visit(const BooleanType& expression) PURE;
  virtual llvm::Type* visit(const VoidType& expression) PURE;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
