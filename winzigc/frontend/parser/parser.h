#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "winzigc/frontend/syntax/kind.h"
#include "winzigc/frontend/syntax/token.h"
#include "winzigc/frontend/ast/program.h"

namespace WinZigC {
namespace Frontend {

class Parser {
public:
  Parser(std::unique_ptr<std::vector<Syntax::Token>> tokens);
  std::unique_ptr<AST::Program> parse();

private:
  bool has_next_token();
  Syntax::Kind peek_next_kind();
  Syntax::Token peek_next_token();
  void go_to_next_token();
  Syntax::Token get_next_token();
  void read(Syntax::Kind kind);

  std::unique_ptr<AST::Program> program;
  std::unique_ptr<std::vector<Syntax::Token>> tokens;
  int token_index;
  std::unique_ptr<Syntax::Token> next_token;

  static const std::map<Syntax::Kind, std::string> kindToString;
};

} // namespace Frontend
} // namespace WinZigC
