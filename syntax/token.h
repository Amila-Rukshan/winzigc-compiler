#pragma once

#include <string>

#include "kind.h"

namespace WinZigC {
namespace Syntax {

struct Token {
  Kind kind;
  std::string lexeme;
  int line;
  int column;
};

} // namespace Syntax
} // namespace WinZigC
