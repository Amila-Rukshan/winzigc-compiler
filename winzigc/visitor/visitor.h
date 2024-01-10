#pragma once

#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/common/pure.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace WinZigC {
namespace Visitor {

class Visitor {
public:
  virtual ~Visitor() = default;

  virtual llvm::Value* visit(const Frontend::AST::IntegerExpression expression) PURE;
  virtual llvm::Value* visit(const Frontend::AST::GlobalVariable expression) PURE;
  virtual llvm::Value* visit(const Frontend::AST::AssignmentExpression expression) PURE;
  virtual llvm::Value* visit(const Frontend::AST::BinaryExpression expression) PURE;

  virtual llvm::Type* visit(const Frontend::AST::IntegerType expression) PURE;
  virtual llvm::Type* visit(const Frontend::AST::BooleanType expression) PURE;
};

} // namespace Visitor
} // namespace WinZigC
