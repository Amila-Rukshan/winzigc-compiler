#pragma once

namespace WinZigC {
namespace Syntax {

/*
 * This enum class is used to represent the kind of a token.
 */
enum class Kind {
  kIdentifier,
  kInteger,
  kTrue,
  kFalse,
  kWhiteSpace,
  kChar,
  kString,
  kLineComment,
  kBlockComment,
  kNewline,           //  \n
  kProgram,           //  program
  kVar,               //  var
  kConst,             //  const
  kType,              //  type
  kFunction,          //  function
  kReturn,            //  return
  kBegin,             //  begin
  kEnd,               //  end
  kSwap,              //  :=:
  kAssign,            //  :=
  kOutput,            //  output
  kIf,                //  if
  kThen,              //  then
  kElse,              //  else
  kWhile,             //  while
  kDo,                //  do
  kCase,              //  case
  kOf,                //  of
  kCaseRange,         //  ..
  kOtherwise,         //  otherwise
  kRepeat,            //  repeat
  kFor,               //  for
  kUntil,             //  until
  kLoop,              //  loop
  kPool,              //  pool
  kExit,              //  exit
  kLessOrEqualOpr,    //  <=
  kNotEqualOpr,       //  <>
  kLessThanOpr,       //  <
  kGreaterOrEqualOpr, //  >=
  kGreaterThanOpr,    //  >
  kEqualToOpr,        //  =
  kModulusOpr,        //  mod
  kAndOpr,            //  and
  kOrOpr,             //  or
  kNotOpr,            //  not
  kRead,              //  read
  kSuccessor,         //  succ
  kPredecessor,       //  pred
  kChr,               //  chr
  kOrd,               //  ord
  kEndOfFile,         //  eof
  kColon,             //  :
  kSemiColon,         //  ;
  kSingleDot,         //  .
  kComma,             //  ,
  kOpenBracket,       //  (
  kCloseBracket,      //  )
  kPlus,              //  +
  kMinus,             //  -
  kMultiply,          //  *
  kDivide,            //  /
  kEndOfProgram,
  kUnknown,
};

} // namespace Syntax
} // namespace WinZigC
