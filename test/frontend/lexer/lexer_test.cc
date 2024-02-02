#include "winzigc/frontend/lexer/lexer.h"
#include "winzigc/frontend/syntax/kind.h"

#include "gtest/gtest.h"

namespace WinZigC {

TEST(LexerTest, LexIntegerToken) {
  Lexer lexer("123");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kInteger);
  ASSERT_EQ(tokens->at(0).lexeme, "123");
}

TEST(LexerTest, LexTrueToken) {
  Lexer lexer("true");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kTrue);
  ASSERT_EQ(tokens->at(0).lexeme, "true");
}

TEST(LexerTest, LexFalseToken) {
  Lexer lexer("false");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kFalse);
  ASSERT_EQ(tokens->at(0).lexeme, "false");
}

TEST(LexerTest, LexWhiteSpaceToken) {
  Lexer lexer(" ");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerTest, LexIdentifierToken) {
  Lexer lexer("abc");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kIdentifier);
  ASSERT_EQ(tokens->at(0).lexeme, "abc");
}

TEST(LexerTest, LexCharToken) {
  Lexer lexer("'a'");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kChar);
  ASSERT_EQ(tokens->at(0).lexeme, "'a'");
}

TEST(LexerTest, LexStringToken) {
  Lexer lexer("\"abc\"");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kString);
  ASSERT_EQ(tokens->at(0).lexeme, "\"abc\"");
}

TEST(LexerTest, LexLineCommentToken) {
  Lexer lexer("# this is line comment\n");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerTest, LexBlockCommentToken) {
  Lexer lexer("{ this is block comment }");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerTest, LexNewLineToken) {
  Lexer lexer("\n\n");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 0);
}

TEST(LexerTest, LexPrgramToken) {
  Lexer lexer("program");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kProgram);
  ASSERT_EQ(tokens->at(0).lexeme, "program");
}

TEST(LexerTest, LexVarToken) {
  Lexer lexer("var");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kVar);
  ASSERT_EQ(tokens->at(0).lexeme, "var");
}

TEST(LexerTest, LexConstToken) {
  Lexer lexer("const");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kConst);
  ASSERT_EQ(tokens->at(0).lexeme, "const");
}

TEST(LexerTest, LexTypeToken) {
  Lexer lexer("type");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kType);
  ASSERT_EQ(tokens->at(0).lexeme, "type");
}

TEST(LexerTest, LexFunctionToken) {
  Lexer lexer("function");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kFunction);
  ASSERT_EQ(tokens->at(0).lexeme, "function");
}

TEST(LexerTest, LexReturnToken) {
  Lexer lexer("return");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kReturn);
  ASSERT_EQ(tokens->at(0).lexeme, "return");
}

TEST(LexerTest, LexBeginToken) {
  Lexer lexer("begin");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kBegin);
  ASSERT_EQ(tokens->at(0).lexeme, "begin");
}

TEST(LexerTest, LexEndToken) {
  Lexer lexer("end");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kEnd);
  ASSERT_EQ(tokens->at(0).lexeme, "end");
}

TEST(LexerTest, LexSwapToken) {
  Lexer lexer(":=:");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kSwap);
  ASSERT_EQ(tokens->at(0).lexeme, ":=:");
}

TEST(LexerTest, LexAssignToken) {
  Lexer lexer(":=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kAssign);
  ASSERT_EQ(tokens->at(0).lexeme, ":=");
}

TEST(LexerTest, LexOutputToken) {
  Lexer lexer("output");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOutput);
  ASSERT_EQ(tokens->at(0).lexeme, "output");
}

TEST(LexerTest, LexIfToken) {
  Lexer lexer("if");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kIf);
  ASSERT_EQ(tokens->at(0).lexeme, "if");
}

TEST(LexerTest, LexThenToken) {
  Lexer lexer("then");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kThen);
  ASSERT_EQ(tokens->at(0).lexeme, "then");
}

TEST(LexerTest, LexElseToken) {
  Lexer lexer("else");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kElse);
  ASSERT_EQ(tokens->at(0).lexeme, "else");
}

TEST(LexerTest, LexWhileToken) {
  Lexer lexer("while");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kWhile);
  ASSERT_EQ(tokens->at(0).lexeme, "while");
}

TEST(LexerTest, LexDoToken) {
  Lexer lexer("do");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kDo);
  ASSERT_EQ(tokens->at(0).lexeme, "do");
}

TEST(LexerTest, LexCaseToken) {
  Lexer lexer("case");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kCase);
  ASSERT_EQ(tokens->at(0).lexeme, "case");
}

TEST(LexerTest, LexOfToken) {
  Lexer lexer("of");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOf);
  ASSERT_EQ(tokens->at(0).lexeme, "of");
}

TEST(LexerTest, LexCaseRangeToken) {
  Lexer lexer("..");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kCaseRange);
  ASSERT_EQ(tokens->at(0).lexeme, "..");
}

TEST(LexerTest, LexOtherwiseToken) {
  Lexer lexer("otherwise");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOtherwise);
  ASSERT_EQ(tokens->at(0).lexeme, "otherwise");
}

TEST(LexerTest, LexRepeatToken) {
  Lexer lexer("repeat");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kRepeat);
  ASSERT_EQ(tokens->at(0).lexeme, "repeat");
}

TEST(LexerTest, LexForToken) {
  Lexer lexer("for");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kFor);
  ASSERT_EQ(tokens->at(0).lexeme, "for");
}

TEST(LexerTest, LexUntilToken) {
  Lexer lexer("until");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kUntil);
  ASSERT_EQ(tokens->at(0).lexeme, "until");
}

TEST(LexerTest, LexLoopToken) {
  Lexer lexer("loop");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kLoop);
  ASSERT_EQ(tokens->at(0).lexeme, "loop");
}

TEST(LexerTest, LexPoolToken) {
  Lexer lexer("pool");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kPool);
  ASSERT_EQ(tokens->at(0).lexeme, "pool");
}

TEST(LexerTest, LexExitToken) {
  Lexer lexer("exit");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kExit);
  ASSERT_EQ(tokens->at(0).lexeme, "exit");
}

TEST(LexerTest, LexLessOPrEqualToken) {
  Lexer lexer("<=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kLessOrEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<=");
}

TEST(LexerTest, LexNotEqualOprToken) {
  Lexer lexer("<>");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kNotEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<>");
}

TEST(LexerTest, LexLessThanOprToken) {
  Lexer lexer("<");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kLessThanOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "<");
}

TEST(LexerTest, LexGreaterOrEqualOprToken) {
  Lexer lexer(">=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kGreaterOrEqualOpr);
  ASSERT_EQ(tokens->at(0).lexeme, ">=");
}

TEST(LexerTest, LexGreaterThanOprToken) {
  Lexer lexer(">");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kGreaterThanOpr);
  ASSERT_EQ(tokens->at(0).lexeme, ">");
}

TEST(LexerTest, LexEqualToOprToken) {
  Lexer lexer("=");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kEqualToOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "=");
}

TEST(LexerTest, LexModulusOprToken) {
  Lexer lexer("mod");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kModulusOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "mod");
}

TEST(LexerTest, LexAndOprToken) {
  Lexer lexer("and");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kAndOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "and");
}

TEST(LexerTest, LexOrOprToken) {
  Lexer lexer("or");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOrOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "or");
}

TEST(LexerTest, LexNotOprToken) {
  Lexer lexer("not");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kNotOpr);
  ASSERT_EQ(tokens->at(0).lexeme, "not");
}

TEST(LexerTest, LexerReadToken) {
  Lexer lexer("read");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kRead);
  ASSERT_EQ(tokens->at(0).lexeme, "read");
}

TEST(LexerTest, LexSuccessorToken) {
  Lexer lexer("succ");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kSuccessor);
  ASSERT_EQ(tokens->at(0).lexeme, "succ");
}

TEST(LexerTest, LexPredecessorToken) {
  Lexer lexer("pred");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kPredecessor);
  ASSERT_EQ(tokens->at(0).lexeme, "pred");
}

TEST(LexerTest, LexChrToken) {
  Lexer lexer("chr");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kChr);
  ASSERT_EQ(tokens->at(0).lexeme, "chr");
}

TEST(LexerTest, LexOrdToken) {
  Lexer lexer("ord");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOrd);
  ASSERT_EQ(tokens->at(0).lexeme, "ord");
}

TEST(LexerTest, LexEofToken) {
  Lexer lexer("eof");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kEndOfFile);
  ASSERT_EQ(tokens->at(0).lexeme, "eof");
}

TEST(LexerTest, LexColonToken) {
  Lexer lexer(":");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kColon);
  ASSERT_EQ(tokens->at(0).lexeme, ":");
}

TEST(LexerTest, LexSemiColonToken) {
  Lexer lexer(";");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kSemiColon);
  ASSERT_EQ(tokens->at(0).lexeme, ";");
}

TEST(LexerTest, LexSingleDotToken) {
  Lexer lexer(".");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kSingleDot);
  ASSERT_EQ(tokens->at(0).lexeme, ".");
}

TEST(LexerTest, LexCommaToken) {
  Lexer lexer(",");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kComma);
  ASSERT_EQ(tokens->at(0).lexeme, ",");
}

TEST(LexerTest, LexOpenBracketToken) {
  Lexer lexer("(");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kOpenBracket);
  ASSERT_EQ(tokens->at(0).lexeme, "(");
}

TEST(LexerTest, LexCloseBracketToken) {
  Lexer lexer(")");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kCloseBracket);
  ASSERT_EQ(tokens->at(0).lexeme, ")");
}

TEST(LexerTest, LexPlusToken) {
  Lexer lexer("+");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kPlus);
  ASSERT_EQ(tokens->at(0).lexeme, "+");
}

TEST(LexerTest, LexMinusToken) {
  Lexer lexer("-");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kMinus);
  ASSERT_EQ(tokens->at(0).lexeme, "-");
}

TEST(LexerTest, LexMultiplyToken) {
  Lexer lexer("*");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kMultiply);
  ASSERT_EQ(tokens->at(0).lexeme, "*");
}

TEST(LexerTest, LexDevideToken) {
  Lexer lexer("/");
  auto tokens = lexer.get_tokens();
  ASSERT_EQ(tokens->size(), 1);
  ASSERT_EQ(tokens->at(0).kind, Syntax::Kind::kDivide);
  ASSERT_EQ(tokens->at(0).lexeme, "/");
}

} // namespace WinZigC
