#include <memory>
#include <string>

#include "winzigc/frontend/ast/expr.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class GlobalVariable {
public:
  GlobalVariable(std::string name, std::unique_ptr<Expression> expression)
      : name(name), expression(std::move(expression)) {}

private:
  std::string name;
  std::unique_ptr<Expression> expression;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
