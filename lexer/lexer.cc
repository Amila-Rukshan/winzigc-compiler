#include "lexer/lexer.h"

namespace WinZigC {
Lexer::Lexer(const std::string& source) {
  this->source = source;
  this->position = 0;
  this->line = 1;
  this->column = 0;
}

char Lexer::get_current_char() {
  if (this->position >= this->source.length()) {
    return '\0';
  }
  return this->source[this->position];
}

} // namespace WinZigC
