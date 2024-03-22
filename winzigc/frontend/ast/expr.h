#pragma once

#include <string>
#include <memory>
#include <utility>

#include "winzigc/common/pure.h"
#include "winzigc/frontend/ast/location.h"

#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Visitor;

enum class UnaryOperation {
  kMinus,
  kPlus,
  kNot,
  kSucc,
  kPred,
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
  Expression(SourceLocation location = {0, 0}) : location(location) {}
  virtual ~Expression() = default;
  virtual llvm::Value* accept(Visitor& visitor) const PURE;
  int get_line() const { return location.line; }
  int get_column() const { return location.column; }

private:
  SourceLocation location;
};

using CaseValue = std::variant<std::unique_ptr<Expression>,
                               std::pair<std::unique_ptr<Expression>, std::unique_ptr<Expression>>>;
using CaseClause = std::pair<AST::CaseValue, std::vector<std::unique_ptr<AST::Expression>>>;

class IntegerExpression : public Expression {
public:
  IntegerExpression(SourceLocation location, int value) : Expression(location), value(value) {}
  int get_value() const { return value; }
  llvm::Value* accept(Visitor& visitor) const override;

private:
  int value;
};

class BooleanExpression : public Expression {
public:
  BooleanExpression(SourceLocation location, bool boolean_val)
      : Expression(location), boolean_val(boolean_val) {}
  bool get_bool() const { return boolean_val; }
  llvm::Value* accept(Visitor& visitor) const override;

private:
  bool boolean_val;
};

class CharacterExpression : public Expression {
public:
  CharacterExpression(SourceLocation location, char character)
      : Expression(location), character(character) {}
  char get_character() const { return character; }
  llvm::Value* accept(Visitor& visitor) const override;

private:
  char character;
};

class CallExpression : public Expression {
public:
  CallExpression(SourceLocation location, std::string name,
                 std::vector<std::unique_ptr<Expression>> arguments)
      : Expression(location), name(name), arguments(std::move(arguments)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }
  const std::vector<std::unique_ptr<Expression>>& get_arguments() const { return arguments; }

private:
  std::string name;
  std::vector<std::unique_ptr<Expression>> arguments;
};

class IdentifierExpression : public Expression {
public:
  IdentifierExpression(SourceLocation location, std::string name)
      : Expression(location), name(name) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const std::string& get_name() const { return name; }

private:
  std::string name;
};

class AssignmentExpression : public Expression {
public:
  AssignmentExpression(SourceLocation location, std::unique_ptr<IdentifierExpression> name,
                       std::unique_ptr<Expression> expression)
      : Expression(location), name(std::move(name)), expression(std::move(expression)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const IdentifierExpression& get_name() const { return *name; }
  const Expression& get_expression() const { return *expression; }

private:
  std::unique_ptr<IdentifierExpression> name;
  std::unique_ptr<Expression> expression;
};

class SwapExpression : public Expression {
public:
  SwapExpression(SourceLocation location, std::unique_ptr<IdentifierExpression> lhs,
                 std::unique_ptr<IdentifierExpression> rhs)
      : Expression(location), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const IdentifierExpression& get_lhs() const { return *lhs; }
  const IdentifierExpression& get_rhs() const { return *rhs; }

private:
  std::unique_ptr<IdentifierExpression> lhs;
  std::unique_ptr<IdentifierExpression> rhs;
};

class BinaryExpression : public Expression {
public:
  BinaryExpression(SourceLocation location, BinaryOperation operation,
                   std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
      : Expression(location), operation(operation), left(std::move(left)), right(std::move(right)) {
  }
  llvm::Value* accept(Visitor& visitor) const override;
  BinaryOperation get_op() const { return operation; }
  const Expression& get_lhs() const { return *left; }
  const Expression& get_rhs() const { return *right; }

private:
  BinaryOperation operation;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

class UnaryExpression : public Expression {
public:
  UnaryExpression(SourceLocation location, UnaryOperation operation,
                  std::unique_ptr<Expression> expression)
      : Expression(location), operation(operation), expression(std::move(expression)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  UnaryOperation get_op() const { return operation; }
  const Expression& get_expression() const { return *expression; }

private:
  UnaryOperation operation;
  std::unique_ptr<Expression> expression;
};

class IfExpression : public Expression {
public:
  IfExpression(SourceLocation location, std::unique_ptr<Expression> condition,
               std::vector<std::unique_ptr<Expression>> then_statement,
               std::vector<std::unique_ptr<Expression>> else_statement)
      : Expression(location), condition(std::move(condition)),
        then_statement(std::move(then_statement)), else_statement(std::move(else_statement)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_condition() const { return *condition; }
  const std::vector<std::unique_ptr<Expression>>& get_then_statement() const {
    return then_statement;
  }
  const std::vector<std::unique_ptr<Expression>>& get_else_statement() const {
    return else_statement;
  }

private:
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Expression>> then_statement;
  std::vector<std::unique_ptr<Expression>> else_statement;
};

class ForExpression : public Expression {
public:
  ForExpression(SourceLocation location, std::unique_ptr<Expression> start_assignment,
                std::unique_ptr<Expression> condition, std::unique_ptr<Expression> end_assignment,
                std::vector<std::unique_ptr<Expression>> statements)
      : Expression(location), start_assignment(std::move(start_assignment)),
        condition(std::move(condition)), end_assignment(std::move(end_assignment)),
        statements(std::move(statements)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_start_assignment() const { return *start_assignment; }
  const Expression& get_condition() const { return *condition; }
  const Expression& get_end_assignment() const { return *end_assignment; }
  const std::vector<std::unique_ptr<Expression>>& get_body_statements() const { return statements; }

private:
  std::unique_ptr<Expression> start_assignment;
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Expression> end_assignment;
  std::vector<std::unique_ptr<Expression>> statements;
};

class RepeatUntilExpression : public Expression {
public:
  RepeatUntilExpression(SourceLocation location, std::unique_ptr<Expression> condition,
                        std::vector<std::unique_ptr<Expression>> statements)
      : Expression(location), condition(std::move(condition)), statements(std::move(statements)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_condition() const { return *condition; }
  const std::vector<std::unique_ptr<Expression>>& get_body_statements() const { return statements; }

private:
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Expression>> statements;
};

class CaseExpression : public Expression {
public:
  CaseExpression(SourceLocation location, std::unique_ptr<Expression> expression,
                 std::vector<CaseClause> cases,
                 std::vector<std::unique_ptr<Expression>> otherwise_clause)
      : Expression(location), expression(std::move(expression)), cases(std::move(cases)),
        otherwise_clause(std::move(otherwise_clause)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_expression() const { return *expression; }
  const std::vector<CaseClause>& get_cases() const { return cases; }
  const std::vector<std::unique_ptr<Expression>>& get_otherwise_clause() const {
    return otherwise_clause;
  }

private:
  std::unique_ptr<Expression> expression;
  std::vector<CaseClause> cases;
  std::vector<std::unique_ptr<Expression>> otherwise_clause;
};

class ReturnExpression : public Expression {
public:
  ReturnExpression(SourceLocation location, std::unique_ptr<Expression> expression)
      : Expression(location), expression(std::move(expression)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_expression() const { return *expression; }

private:
  std::unique_ptr<Expression> expression;
};

class WhileExpression : public Expression {
public:
  WhileExpression(SourceLocation location, std::unique_ptr<Expression> condition,
                  std::vector<std::unique_ptr<Expression>> statements)
      : Expression(location), condition(std::move(condition)), statements(std::move(statements)) {}
  llvm::Value* accept(Visitor& visitor) const override;
  const Expression& get_condition() const { return *condition; }
  const std::vector<std::unique_ptr<Expression>>& get_body_statements() const { return statements; }

private:
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Expression>> statements;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
