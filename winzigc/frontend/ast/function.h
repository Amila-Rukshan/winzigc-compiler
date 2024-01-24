#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/var.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Function {
public:
  Function(std::string name, std::unique_ptr<Type> return_type,
           std::vector<std::unique_ptr<LocalVariable>> parameters,
           std::vector<std::unique_ptr<LocalVariable>> local_var_dclns,
           std::vector<std::unique_ptr<Expression>> function_body_exprs)
      : name(name), return_type(std::move(return_type)), parameters(std::move(parameters)),
        local_var_dclns(std::move(local_var_dclns)),
        function_body_exprs(std::move(function_body_exprs)) {}

private:
  std::string name;
  std::unique_ptr<Type> return_type;
  std::vector<std::unique_ptr<LocalVariable>> parameters;
  std::vector<std::unique_ptr<LocalVariable>> local_var_dclns;
  std::vector<std::unique_ptr<Expression>> function_body_exprs;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
