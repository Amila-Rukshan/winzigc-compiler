#include <memory>

#include "winzigc/frontend/syntax/token.h"

namespace WinZigC {
namespace Frontend {

class Parser {
public:
  Parser(std::unique_ptr<std::vector<Syntax::Token>> tokens) : tokens(std::move(tokens)) {}

private:
  std::unique_ptr<std::vector<Syntax::Token>> tokens;
  private:
    std::unique_ptr<Syntax::Token> token_index;
    std::unique_ptr<Syntax::Token> next_token;
};

} // namespace Frontend
} // namespace WinZigC
