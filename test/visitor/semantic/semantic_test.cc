#include "winzigc/visitor/semantic/semantic_visitor.h"
#include "winzigc/frontend/parser/parser.h"
#include "winzigc/frontend/lexer/lexer.h"

#include "gtest/gtest.h"

namespace WinZigC {

using namespace WinZigC::Frontend;
using namespace WinZigC::Visitor;

TEST(SemanticTest, TestEmptyProgram) {
  Lexer lexer(R"(program winzigc:
    begin
    end 
  winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");
  ASSERT_EQ(errors.size(), 0);
}

TEST(SemanticTest, TestUsingUndeclaredGlobalVariable) {
  Lexer lexer(R"(program winzigc_test:
  var a: integer;
  begin
    read(b);
  end winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 1);
  ASSERT_EQ(errors[0].get_error_message(), ":4:10: Undeclared variable: 'b'");
}

TEST(SemanticTest, TestAssignmentDifferentTypes) {
  Lexer lexer(R"(program winzigc_test:
  var i: integer;
      c: char;
      b: boolean;

  begin
    i := 12;
    i := true;
    i := 'z';

    c := 'c';
    c := false;
    c := 14;

    b := true;
    b := 1;
    b := 'k';
  end winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 6);
  ASSERT_EQ(errors[0].get_error_message(),
            ":8:7: Assignment type mismatch: 'integer' and 'boolean'");
  ASSERT_EQ(errors[1].get_error_message(), ":9:7: Assignment type mismatch: 'integer' and 'char'");
  ASSERT_EQ(errors[2].get_error_message(), ":12:7: Assignment type mismatch: 'char' and 'boolean'");
  ASSERT_EQ(errors[3].get_error_message(), ":13:7: Assignment type mismatch: 'char' and 'integer'");
  ASSERT_EQ(errors[4].get_error_message(),
            ":16:7: Assignment type mismatch: 'boolean' and 'integer'");
  ASSERT_EQ(errors[5].get_error_message(), ":17:7: Assignment type mismatch: 'boolean' and 'char'");
}

TEST(SemanticTest, TestUnaryOperationTypeMismatch) {
  Lexer lexer(R"(program winzigc:
    var I: integer;
        B: boolean;
    begin
      output(------100);
      output(+-+-+-200);
      output( not B);

      output(not I);
      output(-B);
      output( +B);
    end
  winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 3);
  ASSERT_EQ(errors[0].get_error_message(),
            ":9:14: Unary operator 'not' can only be applied to boolean type");
  ASSERT_EQ(errors[1].get_error_message(),
            ":10:14: Unary operator '-' can only be applied to integer type");
  ASSERT_EQ(errors[2].get_error_message(),
            ":11:15: Unary operator '+' can only be applied to integer type");
}

TEST(SemanticTest, TestBinaryOperationTypeMismatch) {
  Lexer lexer(R"(program winzigc:
    var i: integer;
        c: char;
        b: boolean;
    begin
      i := true and 1;
      b := -1 + ('c' or true);

      i :=: b;
      i := (125 + 100 * ( b and c / i) - 1) mod 2;
    end 
  winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 9);

  ASSERT_EQ(errors[0].get_error_message(),
            ":6:17: Binary operator 'and' can only be applied to boolean type");
  ASSERT_EQ(errors[1].get_error_message(),
            ":6:9: Assignment type mismatch: 'integer' and 'boolean'");
  ASSERT_EQ(errors[2].get_error_message(),
            ":7:22: Binary operator 'or' can only be applied to boolean type");
  ASSERT_EQ(errors[3].get_error_message(),
            ":7:15: Binary operator '+' can only be applied to integer type");
  ASSERT_EQ(errors[4].get_error_message(),
            ":7:9: Assignment type mismatch: 'boolean' and 'integer'");
  ASSERT_EQ(errors[5].get_error_message(), ":9:9: Swap type mismatch: 'integer' and 'boolean'");
  ASSERT_EQ(errors[6].get_error_message(),
            ":10:35: Binary operator '/' can only be applied to integer type");
  ASSERT_EQ(errors[7].get_error_message(),
            ":10:29: Binary operator 'and' can only be applied to boolean type");
  ASSERT_EQ(errors[8].get_error_message(),
            ":10:23: Binary operator '*' can only be applied to integer type");
}

TEST(SemanticTest, TestUndeclaredFunctionUsage) {
  Lexer lexer(R"(program winzigc_test:
  var a: integer;

  function foo(bar: integer): integer;
  begin
    return 2 * bar;
  end foo;

  begin
    output(foo(7));  # foo is defined
    output(baz(10)); # baz is not defined
  end winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 1);
  ASSERT_EQ(errors[0].get_error_message(), ":11:12: Calling an undeclared function: 'baz'");
}

TEST(SemanticTest, TestUndeclaredLocalVariable) {
  Lexer lexer(R"(program winzigc:
  var i : integer;
      c : char;
      flag : boolean;

  function GetNext(d:integer):integer;
  begin
    if (flag = true) then
        read(i)
    else
	  read(c);
    flag := not(flag);
  end GetNext;

  function P(d:integer) : integer;
  var v: integer;
  begin
      v := i;
      d:=GetNext(3);
      return (v);
  end P;

  function T(d:integer) : integer;
  var u: integer;  # v is not declared
  begin
    v := P(3);
    while ((c = '*') or (c = '/')) do begin
        if (c = '*') then begin
              d:=GetNext(3);
        v := v * P(3);
        end
        else begin { c = '/' }
            d:=GetNext(3);
            v := v / P(3)
     end;
    end;
    return (v);
  end T;

  begin end winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 6);
  ASSERT_EQ(errors[0].get_error_message(), ":26:5: Undeclared variable: 'v'");
  ASSERT_EQ(errors[1].get_error_message(), ":30:9: Undeclared variable: 'v'");
  ASSERT_EQ(errors[2].get_error_message(), ":30:14: Undeclared variable: 'v'");
  ASSERT_EQ(errors[3].get_error_message(), ":34:13: Undeclared variable: 'v'");
  ASSERT_EQ(errors[4].get_error_message(), ":34:18: Undeclared variable: 'v'");
  ASSERT_EQ(errors[5].get_error_message(), ":37:13: Undeclared variable: 'v'");
}

TEST(SemanticTest, TestFunctionArgumentTypeMismatch) {
  Lexer lexer(R"(program Ackerman:
  var m,n:integer;

  function ackerman(m,n:integer):integer;
  begin
     if m = 0 then return (n + 1)
        else if n = 0 then return (ackerman(m-1,1))
	     else return (ackerman(m-1,ackerman(m,n-1)))
  end ackerman;

  begin
     output(ackerman(false,'c')) # invalid argument types
  end Ackerman.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 2);
  ASSERT_EQ(errors[0].get_error_message(), ":12:22: Function call argument type mismatch, "
                                           "Expected: 'integer', Found: 'boolean' in ackerman");
  ASSERT_EQ(errors[1].get_error_message(), ":12:30: Function call argument type mismatch, "
                                           "Expected: 'integer', Found: 'char' in ackerman");
}

TEST(SemanticTest, TestCaseValueTypeMismatch) {
  Lexer lexer(R"(program winzigc:
  var a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 : integer;
  
  function StoreA ( index, value : integer ):integer;
  begin
    case index of
        1: a1 := value;
        2: a2 := value;
        true: a3 := value; # boolean true is not valid
        4: a4 := value;
        5: a5 := value;
        'y': a6 := value;  # char 'y' is not valid
        7: a7 := value;
        8: a8 := value;
        9: a9 := value;
        10: a10 := value;
        end;
  end StoreA;

  begin
   # do nothing
  end 
  winzigc.)");
  Parser parser(lexer.get_tokens());
  auto program = parser.parse();
  SemanticVisitor semantic_visitor;
  auto errors = semantic_visitor.check(*program, "");

  ASSERT_EQ(errors.size(), 2);
  ASSERT_EQ(errors[0].get_error_message(),
            ":9:9: Case value type mismatch: 'boolean' and 'integer'");
  ASSERT_EQ(errors[1].get_error_message(),
            ":12:11: Case value type mismatch: 'char' and 'integer'");
}

} // namespace WinZigC
