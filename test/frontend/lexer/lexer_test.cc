#include "winzigc/frontend/lexer/lexer.h"
#include "winzigc/frontend/syntax/kind.h"

#include "gtest/gtest.h"

namespace WinZigC {

using namespace WinZigC::Syntax;
void compare_lexer_tokens(std::vector<Token>* exprected_tokens, std::vector<Token>* actual_tokens) {
  ASSERT_EQ(exprected_tokens->size(), actual_tokens->size());
  for (int i = 0; i < exprected_tokens->size(); i++) {
    ASSERT_EQ(exprected_tokens->at(i).kind, actual_tokens->at(i).kind);
    ASSERT_EQ(exprected_tokens->at(i).lexeme, actual_tokens->at(i).lexeme);
  }
}

TEST(LexerSingleTokenTest, LexIntegerToken) {
  Lexer lexer("123");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kInteger);
  ASSERT_EQ(tokens->at(0).lexeme, "123");
}

TEST(LexerSingleTokenTest, LexTrueToken) {
  Lexer lexer("true");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kTrue);
  ASSERT_EQ(tokens->at(0).lexeme, "true");
}

TEST(LexerSingleTokenTest, LexFalseToken) {
  Lexer lexer("false");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kFalse);
  ASSERT_EQ(tokens->at(0).lexeme, "false");
}

TEST(LexerSingleTokenTest, LexWhiteSpaceToken) {
  Lexer lexer(" ");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerSingleTokenTest, LexIdentifierToken) {
  Lexer lexer("abc");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kIdentifier);
  ASSERT_EQ(tokens->at(0).lexeme, "abc");
}

TEST(LexerSingleTokenTest, LexCharToken) {
  Lexer lexer("'a'");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kChar);
  ASSERT_EQ(tokens->at(0).lexeme, "'a'");
}

TEST(LexerSingleTokenTest, LexStringToken) {
  Lexer lexer("\"abc\"");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kString);
  ASSERT_EQ(tokens->at(0).lexeme, "\"abc\"");
}

TEST(LexerSingleTokenTest, LexLineCommentToken) {
  Lexer lexer("# this is line comment\n");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerSingleTokenTest, LexBlockCommentToken) {
  Lexer lexer("{ this is block comment }");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerSingleTokenTest, LexNewLineToken) {
  Lexer lexer("\n\n");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerSingleTokenTest, LexPrgramToken) {
  Lexer lexer("program");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kProgram);
  ASSERT_EQ(tokens->at(0).lexeme, "program");
}

TEST(LexerSingleTokenTest, LexVarToken) {
  Lexer lexer("var");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kVar);
  ASSERT_EQ(tokens->at(0).lexeme, "var");
}

TEST(LexerSingleTokenTest, LexConstToken) {
  Lexer lexer("const");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kConst);
  ASSERT_EQ(tokens->at(0).lexeme, "const");
}

TEST(LexerSingleTokenTest, LexTypeToken) {
  Lexer lexer("type");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kType);
  ASSERT_EQ(tokens->at(0).lexeme, "type");
}

TEST(LexerSingleTokenTest, LexFunctionToken) {
  Lexer lexer("function");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kFunction);
  ASSERT_EQ(tokens->at(0).lexeme, "function");
}

TEST(LexerSingleTokenTest, LexReturnToken) {
  Lexer lexer("return");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kReturn);
  ASSERT_EQ(tokens->at(0).lexeme, "return");
}

TEST(LexerSingleTokenTest, LexBeginToken) {
  Lexer lexer("begin");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kBegin);
  ASSERT_EQ(tokens->at(0).lexeme, "begin");
}

TEST(LexerSingleTokenTest, LexEndToken) {
  Lexer lexer("end");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kEnd);
  ASSERT_EQ(tokens->at(0).lexeme, "end");
}

TEST(LexerSingleTokenTest, LexSwapToken) {
  Lexer lexer(":=:");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSwap);
  ASSERT_EQ(tokens->at(0).lexeme, ":=:");
}

TEST(LexerSingleTokenTest, LexAssignToken) {
  Lexer lexer(":=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kAssign);
  ASSERT_EQ(tokens->at(0).lexeme, ":=");
}

TEST(LexerSingleTokenTest, LexOutputToken) {
  Lexer lexer("output");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOutput);
  ASSERT_EQ(tokens->at(0).lexeme, "output");
}

TEST(LexerSingleTokenTest, LexIfToken) {
  Lexer lexer("if");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kIf);
  ASSERT_EQ(tokens->at(0).lexeme, "if");
}

TEST(LexerSingleTokenTest, LexThenToken) {
  Lexer lexer("then");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kThen);
  ASSERT_EQ(tokens->at(0).lexeme, "then");
}

TEST(LexerSingleTokenTest, LexElseToken) {
  Lexer lexer("else");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kElse);
  ASSERT_EQ(tokens->at(0).lexeme, "else");
}

TEST(LexerSingleTokenTest, LexWhileToken) {
  Lexer lexer("while");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kWhile);
  ASSERT_EQ(tokens->at(0).lexeme, "while");
}

TEST(LexerSingleTokenTest, LexDoToken) {
  Lexer lexer("do");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kDo);
  ASSERT_EQ(tokens->at(0).lexeme, "do");
}

TEST(LexerSingleTokenTest, LexCaseToken) {
  Lexer lexer("case");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kCase);
  ASSERT_EQ(tokens->at(0).lexeme, "case");
}

TEST(LexerSingleTokenTest, LexOfToken) {
  Lexer lexer("of");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOf);
  ASSERT_EQ(tokens->at(0).lexeme, "of");
}

TEST(LexerSingleTokenTest, LexCaseRangeToken) {
  Lexer lexer("..");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kCaseRange);
  ASSERT_EQ(tokens->at(0).lexeme, "..");
}

TEST(LexerSingleTokenTest, LexOtherwiseToken) {
  Lexer lexer("otherwise");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOtherwise);
  ASSERT_EQ(tokens->at(0).lexeme, "otherwise");
}

TEST(LexerSingleTokenTest, LexRepeatToken) {
  Lexer lexer("repeat");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kRepeat);
  ASSERT_EQ(tokens->at(0).lexeme, "repeat");
}

TEST(LexerSingleTokenTest, LexForToken) {
  Lexer lexer("for");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kFor);
  ASSERT_EQ(tokens->at(0).lexeme, "for");
}

TEST(LexerSingleTokenTest, LexUntilToken) {
  Lexer lexer("until");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kUntil);
  ASSERT_EQ(tokens->at(0).lexeme, "until");
}

TEST(LexerSingleTokenTest, LexLoopToken) {
  Lexer lexer("loop");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kLoop);
  ASSERT_EQ(tokens->at(0).lexeme, "loop");
}

TEST(LexerSingleTokenTest, LexPoolToken) {
  Lexer lexer("pool");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kPool);
  ASSERT_EQ(tokens->at(0).lexeme, "pool");
}

TEST(LexerSingleTokenTest, LexExitToken) {
  Lexer lexer("exit");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kExit);
  ASSERT_EQ(tokens->at(0).lexeme, "exit");
}

TEST(LexerSingleTokenTest, LexLessOPrEqualToken) {
  Lexer lexer("<=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kLessOrEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<=");
}

TEST(LexerSingleTokenTest, LexNotEqualOprToken) {
  Lexer lexer("<>");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kNotEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<>");
}

TEST(LexerSingleTokenTest, LexLessThanOprToken) {
  Lexer lexer("<");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kLessThanOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<");
}

TEST(LexerSingleTokenTest, LexGreaterOrEqualOprToken) {
  Lexer lexer(">=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kGreaterOrEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, ">=");
}

TEST(LexerSingleTokenTest, LexGreaterThanOprToken) {
  Lexer lexer(">");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kGreaterThanOpr);
  ASSERT_EQ(tokens->at(0).lexeme, ">");
}

TEST(LexerSingleTokenTest, LexEqualToOprToken) {
  Lexer lexer("=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kEqualToOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "=");
}

TEST(LexerSingleTokenTest, LexModulusOprToken) {
  Lexer lexer("mod");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kModulusOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "mod");
}

TEST(LexerSingleTokenTest, LexAndOprToken) {
  Lexer lexer("and");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kAndOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "and");
}

TEST(LexerSingleTokenTest, LexOrOprToken) {
  Lexer lexer("or");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOrOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "or");
}

TEST(LexerSingleTokenTest, LexNotOprToken) {
  Lexer lexer("not");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kNotOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "not");
}

TEST(LexerSingleTokenTest, LexerReadToken) {
  Lexer lexer("read");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kRead);
  ASSERT_EQ(tokens->at(0).lexeme, "read");
}

TEST(LexerSingleTokenTest, LexSuccessorToken) {
  Lexer lexer("succ");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSuccessor);
  ASSERT_EQ(tokens->at(0).lexeme, "succ");
}

TEST(LexerSingleTokenTest, LexPredecessorToken) {
  Lexer lexer("pred");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kPredecessor);
  ASSERT_EQ(tokens->at(0).lexeme, "pred");
}

TEST(LexerSingleTokenTest, LexChrToken) {
  Lexer lexer("chr");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kChr);
  ASSERT_EQ(tokens->at(0).lexeme, "chr");
}

TEST(LexerSingleTokenTest, LexOrdToken) {
  Lexer lexer("ord");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOrd);
  ASSERT_EQ(tokens->at(0).lexeme, "ord");
}

TEST(LexerSingleTokenTest, LexEofToken) {
  Lexer lexer("eof");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kEndOfFile);
  ASSERT_EQ(tokens->at(0).lexeme, "eof");
}

TEST(LexerSingleTokenTest, LexColonToken) {
  Lexer lexer(":");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kColon);
  ASSERT_EQ(tokens->at(0).lexeme, ":");
}

TEST(LexerSingleTokenTest, LexSemiColonToken) {
  Lexer lexer(";");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSemiColon);
  ASSERT_EQ(tokens->at(0).lexeme, ";");
}

TEST(LexerSingleTokenTest, LexSingleDotToken) {
  Lexer lexer(".");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kSingleDot);
  ASSERT_EQ(tokens->at(0).lexeme, ".");
}

TEST(LexerSingleTokenTest, LexCommaToken) {
  Lexer lexer(",");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kComma);
  ASSERT_EQ(tokens->at(0).lexeme, ",");
}

TEST(LexerSingleTokenTest, LexOpenBracketToken) {
  Lexer lexer("(");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kOpenBracket);
  ASSERT_EQ(tokens->at(0).lexeme, "(");
}

TEST(LexerSingleTokenTest, LexCloseBracketToken) {
  Lexer lexer(")");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kCloseBracket);
  ASSERT_EQ(tokens->at(0).lexeme, ")");
}

TEST(LexerSingleTokenTest, LexPlusToken) {
  Lexer lexer("+");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kPlus);
  ASSERT_EQ(tokens->at(0).lexeme, "+");
}

TEST(LexerSingleTokenTest, LexMinusToken) {
  Lexer lexer("-");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kMinus);
  ASSERT_EQ(tokens->at(0).lexeme, "-");
}

TEST(LexerSingleTokenTest, LexMultiplyToken) {
  Lexer lexer("*");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kMultiply);
  ASSERT_EQ(tokens->at(0).lexeme, "*");
}

TEST(LexerSingleTokenTest, LexDevideToken) {
  Lexer lexer("/");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Kind::kDivide);
  ASSERT_EQ(tokens->at(0).lexeme, "/");
}

TEST(LexerMultiTokenTest, LexSimplestProgram) {
  std::string program =
      R"(program winzigc:
    begin
    end 
  winzigc.)";
  Lexer lexer(program);
  auto actual_tokens = lexer.get_tokens();

  auto expected_tokens = std::make_unique<std::vector<Token>>();
  expected_tokens->push_back(Token{Kind::kProgram, "program"});
  expected_tokens->push_back(Token{Kind::kIdentifier, "winzigc"});
  expected_tokens->push_back(Token{Kind::kColon, ":"});
  expected_tokens->push_back(Token{Kind::kBegin, "begin"});
  expected_tokens->push_back(Token{Kind::kEnd, "end"});
  expected_tokens->push_back(Token{Kind::kIdentifier, "winzigc"});
  expected_tokens->push_back(Token{Kind::kSingleDot, "."});

  compare_lexer_tokens(expected_tokens.get(), actual_tokens.get());
}

TEST(LexerMultiTokenTest, LexProgramWithVarDeclnAndSwapOperation) {
  std::string program =
      R"(program winzigc:
    var a, b: integer;
    begin
      read(a);
      read(b);
      output(a, b);
      a :=: b;
      output(a, b)
    end 
  winzigc.)";
  Lexer lexer(program);
  auto actual_tokens = lexer.get_tokens();

  auto expected_tokens = std::make_unique<std::vector<Token>>();
  expected_tokens->push_back({Kind::kProgram, "program"});
  expected_tokens->push_back({Kind::kIdentifier, "winzigc"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kVar, "var"});
  expected_tokens->push_back({Kind::kIdentifier, "a"});
  expected_tokens->push_back({Kind::kComma, ","});
  expected_tokens->push_back({Kind::kIdentifier, "b"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kIdentifier, "integer"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kBegin, "begin"});
  expected_tokens->push_back({Kind::kRead, "read"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "a"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kRead, "read"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "b"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kOutput, "output"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "a"});
  expected_tokens->push_back({Kind::kComma, ","});
  expected_tokens->push_back({Kind::kIdentifier, "b"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kIdentifier, "a"});
  expected_tokens->push_back({Kind::kSwap, ":=:"});
  expected_tokens->push_back({Kind::kIdentifier, "b"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kOutput, "output"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "a"});
  expected_tokens->push_back({Kind::kComma, ","});
  expected_tokens->push_back({Kind::kIdentifier, "b"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kEnd, "end"});
  expected_tokens->push_back({Kind::kIdentifier, "winzigc"});
  expected_tokens->push_back({Kind::kSingleDot, "."});

  compare_lexer_tokens(expected_tokens.get(), actual_tokens.get());
}

TEST(LexerMultiTokenTest, LexProgramWithManyConstructs) {
  std::string program =
      R"({ This is a program to compute the factors of entered numbers. }
  program factors:

  var i : integer; 

  function Factor (i : integer): integer;
  var
      j : integer;
  begin
      if i > 0 then
        for (j := 1; j <= i; j := j + 1) 
          if i mod j = 0 then output(j)
  end Factor;

  begin
    repeat
      read(i);
      d := Factor(i)
    until i <= 0
  end factors.)";

  Lexer lexer(program);
  auto actual_tokens = lexer.get_tokens();

  auto expected_tokens = std::make_unique<std::vector<Token>>();
  expected_tokens->push_back({Kind::kProgram, "program"});
  expected_tokens->push_back({Kind::kIdentifier, "factors"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kVar, "var"});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kIdentifier, "integer"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kFunction, "function"});
  expected_tokens->push_back({Kind::kIdentifier, "Factor"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kIdentifier, "integer"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kIdentifier, "integer"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kVar, "var"});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kColon, ":"});
  expected_tokens->push_back({Kind::kIdentifier, "integer"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kBegin, "begin"});
  expected_tokens->push_back({Kind::kIf, "if"});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kGreaterThanOpr, ">"});
  expected_tokens->push_back({Kind::kInteger, "0"});
  expected_tokens->push_back({Kind::kThen, "then"});
  expected_tokens->push_back({Kind::kFor, "for"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kAssign, ":="});
  expected_tokens->push_back({Kind::kInteger, "1"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kLessOrEqualOpr, "<="});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kAssign, ":="});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kPlus, "+"});
  expected_tokens->push_back({Kind::kInteger, "1"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kIf, "if"});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kModulusOpr, "mod"});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kEqualToOpr, "="});
  expected_tokens->push_back({Kind::kInteger, "0"});
  expected_tokens->push_back({Kind::kThen, "then"});
  expected_tokens->push_back({Kind::kOutput, "output"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "j"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kEnd, "end"});
  expected_tokens->push_back({Kind::kIdentifier, "Factor"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kBegin, "begin"});
  expected_tokens->push_back({Kind::kRepeat, "repeat"});
  expected_tokens->push_back({Kind::kRead, "read"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kSemiColon, ";"});
  expected_tokens->push_back({Kind::kIdentifier, "d"});
  expected_tokens->push_back({Kind::kAssign, ":="});
  expected_tokens->push_back({Kind::kIdentifier, "Factor"});
  expected_tokens->push_back({Kind::kOpenBracket, "("});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kCloseBracket, ")"});
  expected_tokens->push_back({Kind::kUntil, "until"});
  expected_tokens->push_back({Kind::kIdentifier, "i"});
  expected_tokens->push_back({Kind::kLessOrEqualOpr, "<="});
  expected_tokens->push_back({Kind::kInteger, "0"});
  expected_tokens->push_back({Kind::kEnd, "end"});
  expected_tokens->push_back({Kind::kIdentifier, "factors"});
  expected_tokens->push_back({Kind::kSingleDot, "."});

  compare_lexer_tokens(expected_tokens.get(), actual_tokens.get());
}

} // namespace WinZigC
