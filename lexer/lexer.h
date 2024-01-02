#pragma once

#include <string>
#include <vector>

#include "syntax/token.h"

namespace WinZigC {

constexpr std::array<char, 4> kSpaceCharactors = {' ', '\f', '\r', '\t'};
constexpr std::array<char, 10> kDigitCharacters = {'0', '1', '2', '3', '4',
                                                   '5', '6', '7', '8', '9'};
constexpr std::array<char, 53> kIdentifierCharacters = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'};

class Lexer {
public:
  Lexer(const std::string& source);
  std::vector<Syntax::Token> get_tokens();

private:
  char get_current_char();
  char get_char_at(int);
  std::string find_identifier_or_predefined_syntax();
  // # this is line comment
  Syntax::Token find_line_comment();
  // { this is block comment}
  Syntax::Token find_block_comment();
  Syntax::Token find_new_line();
  Syntax::Token find_white_space();
  Syntax::Token find_integer();
  Syntax::Token find_char();
  Syntax::Token find_string();
  Syntax::Token find_next_token();

  std::vector<Syntax::Token> tokens;
  std::string source;
  std::string lexeme;
  int position;
  int line;
  int column;
};

} // namespace WinZigC
