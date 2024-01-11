#pragma once

#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/var.h"
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
  virtual llvm::Value* visit(const VariableExpression& expression) PURE;
  virtual llvm::Value* visit(const CallExpression& expression) PURE;
  virtual llvm::Value* visit(const IdentifierExpression& expression) PURE;
  virtual llvm::Value* visit(const AssignmentExpression& expression) PURE;
  virtual llvm::Value* visit(const BinaryExpression& expression) PURE;

  virtual llvm::Value* visit(const GlobalVariable& expression) PURE;

  virtual llvm::Type* visit(const IntegerType& expression) PURE;
  virtual llvm::Type* visit(const BooleanType& expression) PURE;
  virtual llvm::Type* visit(const VoidType& expression) PURE;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC