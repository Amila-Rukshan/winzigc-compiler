#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/user_type.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Function {
public:
  Function(int line, std::string name, std::unique_ptr<Type> return_type,
           std::vector<std::unique_ptr<LocalVariable>> parameters,
           std::vector<std::unique_ptr<LocalUserTypeDef>> type_defs,
           std::vector<std::unique_ptr<LocalVariable>> local_var_dclns,
           std::vector<std::unique_ptr<Expression>> function_body_exprs)
      : name(name), return_type(std::move(return_type)), parameters(std::move(parameters)),
        type_defs(std::move(type_defs)), local_var_dclns(std::move(local_var_dclns)),
        function_body_exprs(std::move(function_body_exprs)) {}
  const std::string& get_name() const { return name; }
  const int get_line() const { return line; }
  const Type& get_return_type() const { return *return_type; }
  const std::vector<std::unique_ptr<LocalVariable>>& get_parameters() const { return parameters; }
  const std::vector<std::unique_ptr<LocalVariable>>& get_local_var_dclns() const {
    return local_var_dclns;
  }
  const std::vector<std::unique_ptr<LocalUserTypeDef>>& get_type_defs() const { return type_defs; }
  const std::vector<std::unique_ptr<Expression>>& get_function_body_exprs() const {
    return function_body_exprs;
  }

private:
  int line;
  std::string name;
  std::unique_ptr<Type> return_type;
  std::vector<std::unique_ptr<LocalVariable>> parameters;
  std::vector<std::unique_ptr<LocalUserTypeDef>> type_defs;
  std::vector<std::unique_ptr<LocalVariable>> local_var_dclns;
  std::vector<std::unique_ptr<Expression>> function_body_exprs;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
