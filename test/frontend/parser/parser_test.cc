#include "winzigc/frontend/parser/parser.h"
#include "winzigc/frontend/syntax/kind.h"

#include "gtest/gtest.h"

namespace WinZigC {
namespace Frontend {

using namespace WinZigC::Syntax;
using namespace WinZigC::Frontend::AST;

TEST(ParserTest, ParseSimplestProgramTest) {
  /*
  program winzigc:
      begin
      end
    winzigc.
    */
  auto program_tokens = std::make_unique<std::vector<Token>>();
  program_tokens->push_back(Token{Kind::kProgram, "program"});
  program_tokens->push_back(Token{Kind::kIdentifier, "winzigc"});
  program_tokens->push_back(Token{Kind::kColon, ":"});
  program_tokens->push_back(Token{Kind::kBegin, "begin"});
  program_tokens->push_back(Token{Kind::kEnd, "end"});
  program_tokens->push_back(Token{Kind::kIdentifier, "winzigc"});
  program_tokens->push_back(Token{Kind::kSingleDot, "."});

  Parser parser(std::move(program_tokens));
  std::unique_ptr<Program> program = parser.parse();

  ASSERT_EQ(program->get_name(), "winzigc");
  ASSERT_EQ(program->get_user_types().size(), 0);
  ASSERT_EQ(program->get_variables().size(), 0);
  ASSERT_EQ(program->get_functions().size(), 0);
  ASSERT_EQ(program->get_statements().size(), 0);
}

TEST(ParserTest, ParseProgramWithVarDeclnAndSwapOperation) {
  /*
  program winzigc:
  var a, b: integer;
  begin
    read(a);
    read(b);
    output(a, b);
    a :=: b;
    output(a, b)
  end winzigc.
  */
  auto program_tokens = std::make_unique<std::vector<Token>>();
  program_tokens->push_back({Kind::kProgram, "program"});
  program_tokens->push_back({Kind::kIdentifier, "winzigc"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kVar, "var"});
  program_tokens->push_back({Kind::kIdentifier, "a"});
  program_tokens->push_back({Kind::kComma, ","});
  program_tokens->push_back({Kind::kIdentifier, "b"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kIdentifier, "integer"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kBegin, "begin"});
  program_tokens->push_back({Kind::kRead, "read"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "a"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kRead, "read"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "b"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kOutput, "output"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "a"});
  program_tokens->push_back({Kind::kComma, ","});
  program_tokens->push_back({Kind::kIdentifier, "b"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kIdentifier, "a"});
  program_tokens->push_back({Kind::kSwap, ":=:"});
  program_tokens->push_back({Kind::kIdentifier, "b"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kOutput, "output"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "a"});
  program_tokens->push_back({Kind::kComma, ","});
  program_tokens->push_back({Kind::kIdentifier, "b"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kEnd, "end"});
  program_tokens->push_back({Kind::kIdentifier, "winzigc"});
  program_tokens->push_back({Kind::kSingleDot, "."});

  Parser parser(std::move(program_tokens));
  std::unique_ptr<Program> program = parser.parse();

  ASSERT_EQ(program->get_name(), "winzigc");
  ASSERT_EQ(program->get_user_types().size(), 0);
  ASSERT_EQ(program->get_variables().size(), 2);
  ASSERT_EQ(program->get_functions().size(), 0);
  ASSERT_EQ(program->get_statements().size(), 5);
}

TEST(ParserTest, LexProgramWithManyConstructs) {
  /*
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
    end factors.
  */

  auto program_tokens = std::make_unique<std::vector<Token>>();
  program_tokens->push_back({Kind::kProgram, "program"});
  program_tokens->push_back({Kind::kIdentifier, "factors"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kVar, "var"});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kIdentifier, "integer"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kFunction, "function"});
  program_tokens->push_back({Kind::kIdentifier, "Factor"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kIdentifier, "integer"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kIdentifier, "integer"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kVar, "var"});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kColon, ":"});
  program_tokens->push_back({Kind::kIdentifier, "integer"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kBegin, "begin"});
  program_tokens->push_back({Kind::kIf, "if"});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kGreaterThanOpr, ">"});
  program_tokens->push_back({Kind::kInteger, "0"});
  program_tokens->push_back({Kind::kThen, "then"});
  program_tokens->push_back({Kind::kFor, "for"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kAssign, ":="});
  program_tokens->push_back({Kind::kInteger, "1"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kLessOrEqualOpr, "<="});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kAssign, ":="});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kPlus, "+"});
  program_tokens->push_back({Kind::kInteger, "1"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kIf, "if"});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kModulusOpr, "mod"});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kEqualToOpr, "="});
  program_tokens->push_back({Kind::kInteger, "0"});
  program_tokens->push_back({Kind::kThen, "then"});
  program_tokens->push_back({Kind::kOutput, "output"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "j"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kEnd, "end"});
  program_tokens->push_back({Kind::kIdentifier, "Factor"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kBegin, "begin"});
  program_tokens->push_back({Kind::kRepeat, "repeat"});
  program_tokens->push_back({Kind::kRead, "read"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kSemiColon, ";"});
  program_tokens->push_back({Kind::kIdentifier, "d"});
  program_tokens->push_back({Kind::kAssign, ":="});
  program_tokens->push_back({Kind::kIdentifier, "Factor"});
  program_tokens->push_back({Kind::kOpenBracket, "("});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kCloseBracket, ")"});
  program_tokens->push_back({Kind::kUntil, "until"});
  program_tokens->push_back({Kind::kIdentifier, "i"});
  program_tokens->push_back({Kind::kLessOrEqualOpr, "<="});
  program_tokens->push_back({Kind::kInteger, "0"});
  program_tokens->push_back({Kind::kEnd, "end"});
  program_tokens->push_back({Kind::kIdentifier, "factors"});
  program_tokens->push_back({Kind::kSingleDot, "."});

  Parser parser(std::move(program_tokens));
  std::unique_ptr<Program> program = parser.parse();

  ASSERT_EQ(program->get_name(), "factors");
  ASSERT_EQ(program->get_user_types().size(), 0);
  ASSERT_EQ(program->get_variables().size(), 1);
  ASSERT_EQ(program->get_functions().size(), 1);
  ASSERT_EQ(program->get_statements().size(), 1);

  auto& functions = program->get_functions();
  Function* function = functions.at(0).get();
  ASSERT_EQ(function->get_name(), "Factor");
  ASSERT_EQ(function->get_parameters().size(), 1);
  const Type& typeRef = function->get_return_type();
  ASSERT_NE(dynamic_cast<const IntegerType*>(&typeRef), nullptr);
  ASSERT_EQ(function->get_local_var_dclns().size(), 1);
  ASSERT_EQ(function->get_function_body_exprs().size(), 1);
}

// TODO: Add operator== to all AST classes and compare the ASTs.

} // namespace Frontend
} // namespace WinZigC
