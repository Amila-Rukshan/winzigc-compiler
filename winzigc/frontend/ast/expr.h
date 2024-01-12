#pragma once

#include <string>
#include <memory>
#include <utility>

#include "winzigc/common/pure.h"
#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

enum class UnaryOperation {
  kMinus,
  kPlus,
  kNot,
};

enum class BinaryOperation {
  kMultiply,
  kDivide,
  kModulo,
  kAdd,
  kSubtract,
  kLessThan,
  kLessThanOrEqual,
  kGreaterThan,
  kGreaterThanOrEqual,
  kEqual,
  kNotEqual,
  kAnd,
  kOr,
};

class Expression {
public:
  virtual ~Expression() = default;
  virtual llvm::Value* accept(Visitor& visitor) const PURE;
};

class IntegerExpression : public Expression {
public:
  IntegerExpression(int value) : value(value) {}
  int get_value() const { return value; }
  llvm::Value* accept(Visitor& visitor) const override;

private:
  int value;
};

class VariableExpression : public Expression {
public:
  VariableExpression(std::string name) : name(name) {}
  llvm::Value* accept(Visitor& visitor) const override;

private:
  std::string name;
};

class CallExpression : public Expression {
public:
  CallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
      : name(name), arguments(std::move(arguments)) {}
  llvm::Value* accept(Visitor& visitor) const override;

private:
  std::string name;
  std::vector<std::unique_ptr<Expression>> arguments;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(std::string name) : name(name) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }

private:
  std::string name;
};

class AssignmentExpression : public Expression {
public:
  AssignmentExpression(std::unique_ptr<IdentifierExpression> name,
                       std::unique_ptr<Expression> expression)
      : name(std::move(name)), expression(std::move(expression)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const IdentifierExpression& get_name() const { return *name; }
  const Expression& get_expression() const { return *expression; }

private:
  std::unique_ptr<IdentifierExpression> name;
  std::unique_ptr<Expression> expression;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression(BinaryOperation operation, std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right)
      : operation(operation), left(std::move(left)), right(std::move(right)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  BinaryOperation get_op() const { return operation; }
  const Expression& get_lhs() const { return *left; }
  const Expression& get_rhs() const { return *right; }

private:
  BinaryOperation operation;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
