#pragma once

namespace WinZigC {
namespace Syntax {

enum class Kind {
  IdentifierToken,
  IntegerToken,
  WhiteSpaceToken,
  CharToken,
  StringToken,
  CommentToken,
  NewlineToken,           //  \n
  ProgramToken,           //  program
  VarToken,               //  var
  ConstToken,             //  const
  TypeToken,              //  type
  FunctionToken,          //  function
  ReturnToken,            //  return
  BeginToken,             //  begin
  EndToken,               //  end
  SwapToken,              //  :=:
  AssignToken,            //  :=
  OutputToken,            //  output
  IfToken,                //  if
  ThenToken,              //  then
  ElseToken,              //  else
  WhileToken,             //  while
  DoToken,                //  do
  CaseToken,              //  case
  OfToken,                //  of
  CaseExpToken,           //  ..
  OtherwiseToken,         //  otherwise
  RepeatToken,            //  repeat
  ForToken,               //  for
  UntilToken,             //  until
  LoopToken,              //  loop
  PoolToken,              //  pool
  ExitToken,              //  exit
  LessOrEqualOprToken,    //  <=
  NotEqualOprToken,       //  <>
  LessThanOprToken,       //  <
  GreaterOrEqualOprToken, //  >=
  GreaterThanOprToken,    //  >
  EqualToOprToken,        //  =
  ModulusOprToken,        //  mod
  AndOprToken,            //  and
  OrOprToken,             //  or
  NotOprToken,            //  not
  ReadToken,              //  read
  SuccessorToken,         //  succ
  PredecessorToken,       //  pred
  CharFuncToken,          //  chr
  OrdinalFuncToken,       //  ord
  EndOfFileToken,         //  eof
  ColonToken,             //  :
  SemiColonToken,         //  ;
  SingleDotToken,         //  .
  CommaToken,             //  ,
  OpenBracketToken,       //  (
  CloseBracketToken,      //  )
  PlusToken,              //  +
  MinusToken,             //  -
  MultiplyToken,          //  *
  DivideToken,            //  /
  EndOfProgramToken,
  BadToken,
};

} // namespace Syntax
} // namespace WinZigC
