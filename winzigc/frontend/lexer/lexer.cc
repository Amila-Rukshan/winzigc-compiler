#include <algorithm>
#include <stack>
#include <string>
#include <iostream>
#include <utility>

#include "winzigc/frontend/lexer/lexer.h"

#include "glog/logging.h"
#include "llvm/ADT/APFloat.h"

namespace WinZigC {

#define IS_C_IN_CHARS(c, chars) (std::find(chars.begin(), chars.end(), c) != chars.end())

Lexer::Lexer(const std::string& source) {
  this->source = source;
  tokens = std::make_unique<std::vector<Syntax::Token>>();
  position = 0;
  line = 1;
  column = 0;
}

std::unique_ptr<std::vector<Syntax::Token>> Lexer::get_tokens() {
  llvm::APFloat f(0.0);
  Syntax::Token token = find_next_token();
  while (token.kind != Syntax::Kind::kEndOfProgram) {
    if (token.kind == Syntax::Kind::kUnknown) {
      LOG(ERROR) << "Unknown token: >" << token.lexeme << "< at line: " << token.line
                 << " column: " << token.column;
      break;
    }
    // ignore comments, newlines, whitespaces, and end of file, end of program tokens
    if (token.kind != Syntax::Kind::kWhiteSpace && token.kind != Syntax::Kind::kLineComment &&
        token.kind != Syntax::Kind::kBlockComment && token.kind != Syntax::Kind::kNewline &&
        token.kind != Syntax::Kind::kEndOfFile && token.kind != Syntax::Kind::kEndOfProgram) {
      tokens->push_back(token);
    }
    token = find_next_token();
  }
  return std::move(tokens);
}

char Lexer::get_current_char() {
  if (position >= source.length()) {
    return '\0';
  }
  return source[position];
}

char Lexer::get_char_at(int index) {
  if (index >= source.length()) {
    return '\0';
  }
  return source[index];
}

std::string Lexer::find_identifier_or_predefined_syntax() {
  int start = position;
  if (IS_C_IN_CHARS(get_current_char(), kIdentifierCharacters)) {
    while (IS_C_IN_CHARS(get_current_char(), kIdentifierCharacters) ||
           IS_C_IN_CHARS(get_current_char(), kDigitCharacters)) {
      position++;
      column++;
    }
    return source.substr(start, position - start);
  }
  return "";
}

Syntax::Token Lexer::find_line_comment() {
  int start = position;
  if (get_current_char() == '#') {
    position++;
    column++;
    while (get_current_char() != '\n') {
      position++;
      column++;
    }
    return Syntax::Token{Syntax::Kind::kLineComment, source.substr(start, position - start), line,
                         column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_block_comment() {
  std::stack<char> nested_bracket_stack;
  int start = position;
  if (get_current_char() == '{') {
    position++;
    column++;
    nested_bracket_stack.push('{');
    while (!nested_bracket_stack.empty()) {
      switch (get_current_char()) {
      case '\n':
        line++;
        column = 0;
        break;
      case '{':
        nested_bracket_stack.push('{');
        break;
      case '}':
        if (nested_bracket_stack.top() == '{') {
          nested_bracket_stack.pop();
        } else {
          return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
        }
      }
      position++;
      column++;
    }
    return Syntax::Token{Syntax::Kind::kBlockComment, source.substr(start, position - start), line,
                         column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_new_line() {
  int start = position;
  if (get_current_char() == '\n') {
    position++;
    line++;
    column = 0;
    return Syntax::Token{Syntax::Kind::kNewline, source.substr(start, 1), line, column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_white_space() {
  int start = position;
  if (IS_C_IN_CHARS(get_current_char(), kSpaceCharactors)) {
    position++;
    column++;
    while (IS_C_IN_CHARS(get_current_char(), kSpaceCharactors)) {
      position++;
      column++;
    }
    return Syntax::Token{Syntax::Kind::kWhiteSpace, source.substr(start, position - start), line,
                         column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_integer() {
  int start = position;
  if (IS_C_IN_CHARS(get_current_char(), kDigitCharacters)) {
    position++;
    column++;
    while (IS_C_IN_CHARS(get_current_char(), kDigitCharacters)) {
      position++;
      column++;
    }
    return Syntax::Token{Syntax::Kind::kInteger, source.substr(start, position - start), line,
                         column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_char() {
  int start = position;
  if (get_current_char() == '\'' && get_char_at(position + 1) != '\'' &&
      get_char_at(position + 2) == '\'') {
    position += 3;
    column += 3;
    return Syntax::Token{Syntax::Kind::kChar, source.substr(start, position - start), line, column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_string() {
  int start = position;
  if (get_current_char() == '\"') {
    position++;
    column++;
    while (get_current_char() != '\"') {
      position++;
      column++;
    }
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kString, source.substr(start, position - start), line,
                         column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

Syntax::Token Lexer::find_next_token() {

  if (position >= source.length())
    return Syntax::Token{Syntax::Kind::kEndOfProgram, "", line, column};

  std::string lexeme = find_identifier_or_predefined_syntax();

  if (!lexeme.empty()) {
    if (lexeme == "program") {
      return Syntax::Token{Syntax::Kind::kProgram, "program", line, column};
    } else if (lexeme == "var") {
      return Syntax::Token{Syntax::Kind::kVar, "var", line, column};
    } else if (lexeme == "const") {
      return Syntax::Token{Syntax::Kind::kConst, "const", line, column};
    } else if (lexeme == "type") {
      return Syntax::Token{Syntax::Kind::kType, "type", line, column};
    } else if (lexeme == "function") {
      return Syntax::Token{Syntax::Kind::kFunction, "function", line, column};
    } else if (lexeme == "return") {
      return Syntax::Token{Syntax::Kind::kReturn, "return", line, column};
    } else if (lexeme == "begin") {
      return Syntax::Token{Syntax::Kind::kBegin, "begin", line, column};
    } else if (lexeme == "end") {
      return Syntax::Token{Syntax::Kind::kEnd, "end", line, column};
    } else if (lexeme == "output") {
      return Syntax::Token{Syntax::Kind::kOutput, "output", line, column};
    } else if (lexeme == "if") {
      return Syntax::Token{Syntax::Kind::kIf, "if", line, column};
    } else if (lexeme == "then") {
      return Syntax::Token{Syntax::Kind::kThen, "then", line, column};
    } else if (lexeme == "else") {
      return Syntax::Token{Syntax::Kind::kElse, "else", line, column};
    } else if (lexeme == "while") {
      return Syntax::Token{Syntax::Kind::kWhile, "while", line, column};
    } else if (lexeme == "do") {
      return Syntax::Token{Syntax::Kind::kDo, "do", line, column};
    } else if (lexeme == "case") {
      return Syntax::Token{Syntax::Kind::kCase, "case", line, column};
    } else if (lexeme == "of") {
      return Syntax::Token{Syntax::Kind::kOf, "of", line, column};
    } else if (lexeme == "otherwise") {
      return Syntax::Token{Syntax::Kind::kOtherwise, "otherwise", line, column};
    } else if (lexeme == "repeat") {
      return Syntax::Token{Syntax::Kind::kRepeat, "repeat", line, column};
    } else if (lexeme == "for") {
      return Syntax::Token{Syntax::Kind::kFor, "for", line, column};
    } else if (lexeme == "until") {
      return Syntax::Token{Syntax::Kind::kUntil, "until", line, column};
    } else if (lexeme == "loop") {
      return Syntax::Token{Syntax::Kind::kLoop, "loop", line, column};
    } else if (lexeme == "pool") {
      return Syntax::Token{Syntax::Kind::kPool, "pool", line, column};
    } else if (lexeme == "exit") {
      return Syntax::Token{Syntax::Kind::kExit, "exit", line, column};
    } else if (lexeme == "mod") {
      return Syntax::Token{Syntax::Kind::kModulusOpr, "mod", line, column};
    } else if (lexeme == "or") {
      return Syntax::Token{Syntax::Kind::kOrOpr, "or", line, column};
    } else if (lexeme == "and") {
      return Syntax::Token{Syntax::Kind::kAndOpr, "and", line, column};
    } else if (lexeme == "not") {
      return Syntax::Token{Syntax::Kind::kNotOpr, "not", line, column};
    } else if (lexeme == "read") {
      return Syntax::Token{Syntax::Kind::kRead, "read", line, column};
    } else if (lexeme == "succ") {
      return Syntax::Token{Syntax::Kind::kSuccessor, "succ", line, column};
    } else if (lexeme == "pred") {
      return Syntax::Token{Syntax::Kind::kPredecessor, "pred", line, column};
    } else if (lexeme == "chr") {
      return Syntax::Token{Syntax::Kind::kCharFunc, "chr", line, column};
    } else if (lexeme == "ord") {
      return Syntax::Token{Syntax::Kind::kOrdinalFunc, "ord", line, column};
    } else if (lexeme == "eof") {
      return Syntax::Token{Syntax::Kind::kEndOfFile, "eof", line, column};
    } else {
      return Syntax::Token{Syntax::Kind::kIdentifier, lexeme, line, column};
    }
  }

  // line comment
  Syntax::Token token = find_line_comment();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // block comment
  token = find_block_comment();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // new line
  token = find_new_line();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // white space
  token = find_white_space();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // integer
  token = find_integer();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // char
  token = find_char();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // string
  token = find_string();
  if (token.kind != Syntax::Kind::kUnknown)
    return token;

  // swap :=:
  if (get_current_char() == ':' && get_char_at(position + 1) == '=' &&
      get_char_at(position + 2) == ':') {
    position += 3;
    column += 3;
    return Syntax::Token{Syntax::Kind::kSwap, ":=:", line, column};
  }

  // ":=", "..", "<=", "<>", ">="
  if (get_current_char() == ':' && get_char_at(position + 1) == '=') {
    position += 2;
    column += 2;
    return Syntax::Token{Syntax::Kind::kAssign, ":=", line, column};
  } else if (get_current_char() == '.' && get_char_at(position + 1) == '.') {
    position += 2;
    column += 2;
    return Syntax::Token{Syntax::Kind::kCaseExp, "..", line, column};
  } else if (get_current_char() == '<' && get_char_at(position + 1) == '=') {
    position += 2;
    column += 2;
    return Syntax::Token{Syntax::Kind::kLessOrEqualOpr, "<=", line, column};
  } else if (get_current_char() == '<' && get_char_at(position + 1) == '>') {
    position += 2;
    column += 2;
    return Syntax::Token{Syntax::Kind::kNotEqualOpr, "<>", line, column};
  } else if (get_current_char() == '>' && get_char_at(position + 1) == '=') {
    position += 2;
    column += 2;
    return Syntax::Token{Syntax::Kind::kGreaterOrEqualOpr, ">=", line, column};
  }

  //  ":", ".", "<", ">", "=", ";", ",", "(", ")", "+", "-", "*", "/"
  switch (get_current_char()) {
  case ':':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kColon, ":", line, column};
  case '.':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kSingleDot, ".", line, column};
  case '<':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kLessThanOpr, "<", line, column};
  case '>':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kGreaterThanOpr, ">", line, column};
  case '=':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kEqualToOpr, "=", line, column};
  case ';':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kSemiColon, ";", line, column};
  case ',':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kComma, ",", line, column};
  case '(':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kOpenBracket, "(", line, column};
  case ')':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kCloseBracket, ")", line, column};
  case '+':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kPlus, "+", line, column};
  case '-':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kMinus, "-", line, column};
  case '*':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kMultiply, "*", line, column};
  case '/':
    position++;
    column++;
    return Syntax::Token{Syntax::Kind::kDivide, "/", line, column};
  }
  return Syntax::Token{Syntax::Kind::kUnknown, "", line, column};
}

} // namespace WinZigC
