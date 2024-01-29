#pragma once

#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "winzigc/frontend/ast/function.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/expr.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Program {
public:
  Program(std::string name, std::vector<std::unique_ptr<GlobalVariable>> vars,
          std::vector<std::unique_ptr<Function>> functions,
          std::vector<std::unique_ptr<Expression>> statements)
      : name(name), variables(std::move(vars)), functions(std::move(functions)),
        statements(std::move(statements)) {
    discard_variable =
        std::make_unique<GlobalVariable>(std::string("d"), std::make_unique<IntegerType>());
  }

  const std::string& get_name() const { return name; }
  const std::vector<std::unique_ptr<GlobalVariable>>& get_variables() const { return variables; }
  const std::vector<std::unique_ptr<Function>>& get_functions() const { return functions; }
  const std::vector<std::unique_ptr<Expression>>& get_statements() const { return statements; }
  const std::unique_ptr<GlobalVariable>& get_discard_variable() const { return discard_variable; }

private:
  std::string name;
  std::vector<std::unique_ptr<GlobalVariable>> variables;
  std::unique_ptr<GlobalVariable> discard_variable;
  std::vector<std::unique_ptr<Function>> functions;
  std::vector<std::unique_ptr<Expression>> statements;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
