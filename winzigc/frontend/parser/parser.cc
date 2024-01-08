#include <memory>

#include "winzigc/frontend/syntax/kind.h"
#include "winzigc/frontend/parser/parser.h"
#include "winzigc/frontend/syntax/token.h"

#include "glog/logging.h"

namespace WinZigC {
namespace Frontend {

Parser::Parser(std::unique_ptr<std::vector<Syntax::Token>> tokens) : tokens(std::move(tokens)) {
  token_index = 0;
  program = nullptr;
}

bool Parser::has_next_token() { return token_index < tokens->size(); }

Syntax::Kind Parser::peek_next_kind() {
  if (has_next_token()) {
    return tokens->at(token_index).kind;
  } else {
    throw std::runtime_error("No more tokens to peek");
    return Syntax::Kind::kUnknown;
  }
}

Syntax::Token Parser::peek_next_token() {
  if (has_next_token()) {
    return tokens->at(token_index);
  } else {
    throw std::runtime_error("No more tokens to peek");
    return Syntax::Token{Syntax::Kind::kUnknown, "", 0, 0};
  }
}

void Parser::go_to_next_token() {
  if (has_next_token()) {
    token_index++;
  } else {
    throw std::runtime_error("No more tokens to peek");
  }
}

Syntax::Token Parser::get_next_token() {
  if (has_next_token()) {
    return tokens->at(token_index++);
  } else {
    throw std::runtime_error("No more tokens to peek");
    return Syntax::Token{Syntax::Kind::kUnknown, "", 0, 0};
  }
}

void Parser::read(Syntax::Kind kind) {
  if (has_next_token()) {
    if (tokens->at(token_index).kind == kind) {
      token_index++;
    } else {
      LOG(ERROR) << "Expected token: " << kindToString.at(kind);
      LOG(ERROR) << "Actual token: " << kindToString.at(tokens->at(token_index).kind);
      LOG(ERROR) << "line: " << tokens->at(token_index).line;
      LOG(ERROR) << "column: " << tokens->at(token_index).column;
      throw std::runtime_error("Unexpected token");
    }
  } else {
    throw std::runtime_error("No more tokens to peek");
  }
}

std::unique_ptr<AST::Program> Parser::parse() { return nullptr; }

const std::map<Syntax::Kind, std::string> Parser::kindToString = {
    {Syntax::Kind::kIdentifier, "kIdentifier"},
    {Syntax::Kind::kInteger, "kInteger"},
    {Syntax::Kind::kWhiteSpace, "kWhiteSpace"},
    {Syntax::Kind::kChar, "kChar"},
    {Syntax::Kind::kString, "kString"},
    {Syntax::Kind::kLineComment, "kLineComment"},
    {Syntax::Kind::kBlockComment, "kBlockComment"},
    {Syntax::Kind::kNewline, "kNewline"},
    {Syntax::Kind::kProgram, "kProgram"},
    {Syntax::Kind::kVar, "kVar"},
    {Syntax::Kind::kConst, "kConst"},
    {Syntax::Kind::kType, "kType"},
    {Syntax::Kind::kFunction, "kFunction"},
    {Syntax::Kind::kReturn, "kReturn"},
    {Syntax::Kind::kBegin, "kBegin"},
    {Syntax::Kind::kEnd, "kEnd"},
    {Syntax::Kind::kSwap, "kSwap"},
    {Syntax::Kind::kAssign, "kAssign"},
    {Syntax::Kind::kOutput, "kOutput"},
    {Syntax::Kind::kIf, "kIf"},
    {Syntax::Kind::kThen, "kThen"},
    {Syntax::Kind::kElse, "kElse"},
    {Syntax::Kind::kWhile, "kWhile"},
    {Syntax::Kind::kDo, "kDo"},
    {Syntax::Kind::kCase, "kCase"},
    {Syntax::Kind::kOf, "kOf"},
    {Syntax::Kind::kCaseExp, "kCaseExp"},
    {Syntax::Kind::kOtherwise, "kOtherwise"},
    {Syntax::Kind::kRepeat, "kRepeat"},
    {Syntax::Kind::kFor, "kFor"},
    {Syntax::Kind::kUntil, "kUntil"},
    {Syntax::Kind::kLoop, "kLoop"},
    {Syntax::Kind::kPool, "kPool"},
    {Syntax::Kind::kExit, "kExit"},
    {Syntax::Kind::kLessOrEqualOpr, "kLessOrEqualOpr"},
    {Syntax::Kind::kNotEqualOpr, "kNotEqualOpr"},
    {Syntax::Kind::kLessThanOpr, "kLessThanOpr"},
    {Syntax::Kind::kGreaterOrEqualOpr, "kGreaterOrEqualOpr"},
    {Syntax::Kind::kGreaterThanOpr, "kGreaterThanOpr"},
    {Syntax::Kind::kEqualToOpr, "kEqualToOpr"},
    {Syntax::Kind::kModulusOpr, "kModulusOpr"},
    {Syntax::Kind::kAndOpr, "kAndOpr"},
    {Syntax::Kind::kOrOpr, "kOrOpr"},
    {Syntax::Kind::kNotOpr, "kNotOpr"},
    {Syntax::Kind::kRead, "kRead"},
    {Syntax::Kind::kSuccessor, "kSuccessor"},
    {Syntax::Kind::kPredecessor, "kPredecessor"},
    {Syntax::Kind::kCharFunc, "kCharFunc"},
    {Syntax::Kind::kOrdinalFunc, "kOrdinalFunc"},
    {Syntax::Kind::kEndOfFile, "kEndOfFile"},
    {Syntax::Kind::kColon, "kColon"},
    {Syntax::Kind::kSemiColon, "kSemiColon"},
    {Syntax::Kind::kSingleDot, "kSingleDot"},
    {Syntax::Kind::kComma, "kComma"},
    {Syntax::Kind::kOpenBracket, "kOpenBracket"},
    {Syntax::Kind::kCloseBracket, "kCloseBracket"},
    {Syntax::Kind::kPlus, "kPlus"},
    {Syntax::Kind::kMinus, "kMinus"},
    {Syntax::Kind::kMultiply, "kMultiply"},
    {Syntax::Kind::kDivide, "kDivide"},
    {Syntax::Kind::kEndOfProgram, "kEndOfProgram"},
    {Syntax::Kind::kUnknown, "kUnknown"},
};

} // namespace Frontend
} // namespace WinZigC
