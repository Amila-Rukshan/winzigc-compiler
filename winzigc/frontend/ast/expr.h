#pragma once

#include <string>
#include <memory>
#include <utility>

namespace WinZigC {
namespace Frontend {
namespace AST {

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
};

class IntegerExpression : public Expression {
public:
  IntegerExpression(int value) : value(value) {}

private:
  int value;
};

class VariableExpression : public Expression {
public:
  VariableExpression(std::string name) : name(name) {}

private:
  std::string name;
};

class CallExpression : public Expression {
public:
  CallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
      : name(name), arguments(std::move(arguments)) {}

private:
  std::string name;
  std::vector<std::unique_ptr<Expression>> arguments;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(std::string name) : name(name) {}

private:
  std::string name;
};

class AssignmentExpression : public Expression {
public:
  AssignmentExpression(std::unique_ptr<IdentifierExpression> name, std::unique_ptr<Expression> expression)
      : name(std::move(name)), expression(std::move(expression)) {}

private:
  std::unique_ptr<IdentifierExpression> name;
  std::unique_ptr<Expression> expression;
};

class UnaryExpression : public Expression {
public:
  UnaryExpression(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

private:
  std::unique_ptr<Expression> expression;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression(BinaryOperation operation, std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right)
      : operation(operation), left(std::move(left)), right(std::move(right)) {}

private:
  BinaryOperation operation;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
