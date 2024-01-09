#pragma once

#include <memory>
#include <string>
#include <utility>

#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/type.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class GlobalVariable {
public:
  GlobalVariable(std::string name, std::unique_ptr<Type> type)
      : name(name), type(std::move(type)) {}

private:
  std::string name;
  std::unique_ptr<Type> type;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
