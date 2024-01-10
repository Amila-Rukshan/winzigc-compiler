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
        statements(std::move(statements)) {}

private:
  std::string name;
  std::vector<std::unique_ptr<GlobalVariable>> variables;
  std::vector<std::unique_ptr<Function>> functions;
  std::vector<std::unique_ptr<Expression>> statements;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
