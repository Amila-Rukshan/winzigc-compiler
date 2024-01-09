#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/statement.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Parameter {
public:
  Parameter(std::string name, std::unique_ptr<Type> type) : name(name), type(std::move(type)) {}

private:
  std::string name;
  std::unique_ptr<Type> type;
};

class Function {
public:
  Function(std::string name, std::unique_ptr<Type> return_type,
           std::vector<std::unique_ptr<Parameter>> parameters,
           std::vector<std::unique_ptr<Statement>> statements)
      : name(name), return_type(std::move(return_type)), parameters(std::move(parameters)),
        statements(std::move(statements)) {}

private:
  std::string name;
  std::unique_ptr<Type> return_type;
  std::vector<std::unique_ptr<Parameter>> parameters;
  std::vector<std::unique_ptr<Statement>> statements;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
