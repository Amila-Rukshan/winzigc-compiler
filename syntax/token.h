#pragma once

#include <string>

#include "kind.h"

namespace WinZigC {
namespace Syntax {

/*
 * This struct is used to represent a token.
 */
struct Token {
  Kind kind;
  std::string lexeme;
  int line;
  int column;
};

} // namespace Syntax
} // namespace WinZigC
