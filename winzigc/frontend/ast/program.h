#include <string>
#include <memory>

#include "winzigc/frontend/ast/function.h"
#include "winzigc/frontend/ast/var.h"

namespace WinZigC {
namespace Frontend {
namespace AST {

class Program {
public:
  Program(std::string name, std::vector<std::unique_ptr<GlobalVariable>> vars,
          std::vector<std::unique_ptr<Function>> functions,
          std::vector<std::unique_ptr<Statement>> statements)
      : name(std::move(name)), variables(std::move(vars)), functions(std::move(functions)) {}

private:
  std::string name;
  std::vector<std::unique_ptr<GlobalVariable>> variables;
  std::vector<std::unique_ptr<Function>> functions;
};

} // namespace AST
} // namespace Frontend
} // namespace WinZigC
