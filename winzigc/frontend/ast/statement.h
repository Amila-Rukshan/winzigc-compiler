#include <memory>
#include <string>

#include "winzigc/frontend/ast/expr.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Statement {
public:
  virtual ~Statement() = default;
};

class AssignmentStatement : public Statement {
public:
  AssignmentStatement(std::string name, std::unique_ptr<Expression> expression)
      : name(name), expression(std::move(expression)) {}

private:
  std::string name;
  std::unique_ptr<Expression> expression;
};

class OutputStatement : public Statement {
public:
  OutputStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

private:
  std::unique_ptr<Expression> expression;
};

class IfStatement : public Statement {
public:
  IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> then_statement,
              std::unique_ptr<Statement> else_statement)
      : condition(std::move(condition)), then_statement(std::move(then_statement)),
        else_statement(std::move(else_statement)) {}

private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> then_statement;
  std::unique_ptr<Statement> else_statement;
};

class WhileStatement : public Statement {
public:
  WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> statement)
      : condition(std::move(condition)), statement(std::move(statement)) {}

private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> statement;
};

class RepeatStatement : public Statement {
public:
  RepeatStatement(std::unique_ptr<Expression> condition,
                  std::vector<std::unique_ptr<Statement>> statements)
      : condition(std::move(condition)), statements(std::move(statements)) {}

private:
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Statement>> statements;
};

class ForStatement : public Statement {
public:
  ForStatement(std::unique_ptr<AssignmentStatement> start, std::unique_ptr<Expression> condition,
               std::unique_ptr<AssignmentStatement> end, std::unique_ptr<Statement> statement)
      : start(std::move(start)), condition(std::move(condition)), end(std::move(end)),
        statement(std::move(statement)) {}

private:
  std::unique_ptr<AssignmentStatement> start;
  std::unique_ptr<Expression> condition;
  std::unique_ptr<AssignmentStatement> end;
  std::unique_ptr<Statement> statement;
};

class LoopStatement : public Statement {
public:
  LoopStatement(std::unique_ptr<Statement> statement) : statement(std::move(statement)) {}

private:
  std::unique_ptr<Statement> statement;
};

class ReadStatement : public Statement {
public:
  ReadStatement(std::string name) : name(name) {}

private:
  std::string name;
};

class ExitStatement : public Statement {
public:
  ExitStatement() {}
};

class ReturnStatement : public Statement {
public:
  ReturnStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

private:
  std::unique_ptr<Expression> expression;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
