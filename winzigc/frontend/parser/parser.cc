#include <memory>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "winzigc/frontend/syntax/kind.h"
#include "winzigc/frontend/parser/parser.h"
#include "winzigc/frontend/syntax/token.h"
#include "winzigc/frontend/ast/program.h"
#include "winzigc/frontend/ast/expr.h"
#include "winzigc/frontend/ast/var.h"
#include "winzigc/frontend/ast/statement.h"
#include "winzigc/frontend/ast/type.h"
#include "winzigc/frontend/ast/function.h"

#include "glog/logging.h"

namespace WinZigC {
namespace Frontend {

Parser::Parser(std::unique_ptr<std::vector<Syntax::Token>> tokens) : tokens(std::move(tokens)) {
  token_index = 0;
  program = nullptr;
}

std::unique_ptr<AST::Program> Parser::parse() {
  read(Syntax::Kind::kProgram);
  std::string program_name = read(Syntax::Kind::kIdentifier);
  read(Syntax::Kind::kColon);
  // TODO: consts
  // TODO: types
  std::vector<std::unique_ptr<AST::GlobalVariable>> var_dclns = parse_dclns();
  std::vector<std::unique_ptr<AST::Function>> functions;
  std::vector<std::unique_ptr<AST::Expression>> statements = parse_body();

  return std::make_unique<AST::Program>(program_name, std::move(var_dclns), std::move(functions),
                                        std::move(statements));
}

// Dclns      ->  'var' (Dcln ';')+                                             => "dclns"
//            ->                                                                => "dclns";
std::vector<std::unique_ptr<AST::GlobalVariable>> Parser::parse_dclns() {
  std::vector<std::unique_ptr<AST::GlobalVariable>> var_dclns;
  if (current_token->kind == Syntax::Kind::kVar) {
    read(Syntax::Kind::kVar);
    parse_dcln(var_dclns);
    read(Syntax::Kind::kSemiColon);
    while (current_token->kind == Syntax::Kind::kIdentifier) {
      parse_dcln(var_dclns);
      read(Syntax::Kind::kSemiColon);
    }
  }
  return var_dclns;
}

// Dcln       ->  Name list ',' ':' Name                                        => "var";
void Parser::parse_dcln(std::vector<std::unique_ptr<AST::GlobalVariable>>& var_dclns) {
  std::vector<std::string> identifiers;
  identifiers.push_back(read(Syntax::Kind::kIdentifier));
  while (current_token->kind != Syntax::Kind::kColon) {
    read(Syntax::Kind::kComma);
    identifiers.push_back(read(Syntax::Kind::kIdentifier));
  }
  read(Syntax::Kind::kColon);
  std::string type = read(Syntax::Kind::kIdentifier);
  for (auto identifier : identifiers) {
    var_dclns.push_back(std::make_unique<AST::GlobalVariable>(identifier, create_type(type)));
  }
}

// Body       ->  'begin' Statement list ';' 'end'                              => "block";
std::vector<std::unique_ptr<AST::Expression>> Parser::parse_body() {
  std::vector<std::unique_ptr<AST::Expression>> statements;
  read(Syntax::Kind::kBegin);
  parse_statement(statements);
  while (current_token->kind == Syntax::Kind::kSemiColon) {
    read(Syntax::Kind::kSemiColon);
    parse_statement(statements);
  }
  read(Syntax::Kind::kEnd);
  return statements;
}

// Statement  ->  Assignment
//            ->  'output' '(' OutExp list ',' ')'                              => "output"
//            ->  'if' Expression 'then' Statement ('else' Statement)?          => "if"
//            ->  'while' Expression 'do' Statement                             => "while"
//            ->  'repeat' Statement list ';' 'until' Expression                => "repeat"
//            ->  'for' '(' ForStat ';' ForExp ';' ForStat ')' Statement        => "for"
//            ->  'loop' Statement list ';' 'pool'                              => "loop"
//            ->  'case' Expression 'of' Caseclauses OtherwiseClause 'end'      => "case"
//            ->  'read' '(' Name list ',' ')'                                  => "read"
//            ->  'exit'                                                        => "exit"
//            ->  'return' Expression                                           => "return"
//            ->  Body
//            ->                                                                => "ᐸnullᐳ";
void Parser::parse_statement(std::vector<std::unique_ptr<AST::Expression>>& statements) {
  switch (current_token->kind) {
  case Syntax::Kind::kIdentifier:
    parse_assignment_statement(statements);
    break;
  case Syntax::Kind::kOutput:
    parse_output_statement(statements);
    break;
  case Syntax::Kind::kRead:
    parse_input_statement(statements);
    break;
  case Syntax::Kind::kIf:
    parse_if_statement(statements);
    break;
  default:
    break;
  }
}

// Assignment ->  Name ':=' Expression                                          => "assign"
//            ->  Name ':=:' Name                                               => "swap";
void Parser::parse_assignment_statement(std::vector<std::unique_ptr<AST::Expression>>& statements) {
  auto identifier_expr =
      std::make_unique<AST::IdentifierExpression>(read(Syntax::Kind::kIdentifier));
  std::unique_ptr<AST::IdentifierExpression> identifier_expr_rhs;
  std::unique_ptr<AST::Expression> expression;
  switch (current_token->kind) {
  case Syntax::Kind::kAssign:
    read(Syntax::Kind::kAssign);
    expression = parse_expression();
    statements.push_back(std::make_unique<AST::AssignmentExpression>(std::move(identifier_expr),
                                                                     std::move(expression)));
    break;
  case Syntax::Kind::kSwap:
    read(Syntax::Kind::kSwap);
    identifier_expr_rhs =
        std::make_unique<AST::IdentifierExpression>(read(Syntax::Kind::kIdentifier));
    statements.push_back(std::make_unique<AST::SwapExpression>(std::move(identifier_expr),
                                                               std::move(identifier_expr_rhs)));
    break;
  default:
    throw std::runtime_error("Invalid assignment statement");
    break;
  }
}

void Parser::parse_output_statement(std::vector<std::unique_ptr<AST::Expression>>& statements) {
  std::string name = read(Syntax::Kind::kOutput); // TODO: make this printf for lib function
  read(Syntax::Kind::kOpenBracket);
  std::vector<std::unique_ptr<AST::Expression>> arguments;
  if (current_token->kind != Syntax::Kind::kCloseBracket) {
    arguments.push_back(parse_expression());
    while (current_token->kind == Syntax::Kind::kComma) {
      read(Syntax::Kind::kComma);
      arguments.push_back(parse_expression());
    }
  }
  read(Syntax::Kind::kCloseBracket);
  statements.push_back(std::make_unique<AST::CallExpression>(name, std::move(arguments)));
}

void Parser::parse_input_statement(std::vector<std::unique_ptr<AST::Expression>>& statements) {
  std::string name = read(Syntax::Kind::kRead);
  read(Syntax::Kind::kOpenBracket);
  std::vector<std::unique_ptr<AST::Expression>> arguments;
  if (current_token->kind != Syntax::Kind::kCloseBracket) {
    arguments.push_back(parse_expression());
    while (current_token->kind == Syntax::Kind::kComma) {
      read(Syntax::Kind::kComma);
      arguments.push_back(parse_expression());
    }
  }
  read(Syntax::Kind::kCloseBracket);
  statements.push_back(std::make_unique<AST::CallExpression>(name, std::move(arguments)));
}

void Parser::parse_if_statement(std::vector<std::unique_ptr<AST::Expression>>& statements) {
  read(Syntax::Kind::kIf);
  std::unique_ptr<AST::Expression> expression = parse_expression();
  read(Syntax::Kind::kThen);
  std::vector<std::unique_ptr<AST::Expression>> if_block_statements;
  parse_statement(if_block_statements);
  std::vector<std::unique_ptr<AST::Expression>> else_block_statements;
  if (current_token->kind == Syntax::Kind::kElse) {
    read(Syntax::Kind::kElse);
    parse_statement(else_block_statements);
  }
  statements.push_back(std::make_unique<AST::IfExpression>(
      std::move(expression), std::move(if_block_statements), std::move(else_block_statements)));
}

std::unique_ptr<AST::Expression> Parser::parse_expression() {
  auto expression_lhs = parse_primary();
  if (!expression_lhs) {
    return nullptr;
  }
  return parse_binary_rhs(0, std::move(expression_lhs));
}

std::unique_ptr<AST::Expression> Parser::parse_binary_rhs(int precedence,
                                                          std::unique_ptr<AST::Expression> lhs) {
  while (true) {
    int token_precedence = get_token_precedence();
    if (token_precedence < precedence) {
      return lhs;
    }
    AST::BinaryOperation binary_operator = get_binary_operation(current_token->kind);
    go_to_next_token();
    std::unique_ptr<AST::Expression> rhs = parse_primary();
    if (!rhs) {
      return nullptr;
    }
    int next_precedence = get_token_precedence();
    if (token_precedence < next_precedence) {
      rhs = parse_binary_rhs(token_precedence + 1, std::move(rhs));
      if (!rhs) {
        return nullptr;
      }
    }
    lhs = std::make_unique<AST::BinaryExpression>(binary_operator, std::move(lhs), std::move(rhs));
  }
}

std::unique_ptr<AST::Expression> Parser::parse_primary() {
  std::unique_ptr<AST::Expression> expr;
  switch (current_token->kind) {
  case Syntax::Kind::kIdentifier:
    if (peek_next_kind() == Syntax::Kind::kOpenBracket) {
      std::string name = read(Syntax::Kind::kIdentifier);
      read(Syntax::Kind::kOpenBracket);
      std::vector<std::unique_ptr<AST::Expression>> arguments;
      if (current_token->kind != Syntax::Kind::kCloseBracket) {
        arguments.push_back(parse_expression());
        while (current_token->kind == Syntax::Kind::kComma) {
          read(Syntax::Kind::kComma);
          arguments.push_back(parse_expression());
        }
      }
      read(Syntax::Kind::kCloseBracket);
      return std::make_unique<AST::CallExpression>(name, std::move(arguments));
    }
    return std::make_unique<AST::IdentifierExpression>(read(Syntax::Kind::kIdentifier));
  case Syntax::Kind::kInteger:
    return std::make_unique<AST::IntegerExpression>(std::stoi(read(Syntax::Kind::kInteger)));
  case Syntax::Kind::kOpenBracket:
    read(Syntax::Kind::kOpenBracket);
    expr = parse_expression();
    read(Syntax::Kind::kCloseBracket);
    return expr;
  default:
    throw std::runtime_error("Invalid primary expression");
    return nullptr;
  }
}

int Parser::get_token_precedence() {
  if (kind_to_precedence.find(current_token->kind) != kind_to_precedence.end()) {
    return kind_to_precedence.at(current_token->kind);
  } else {
    return -1;
  }
}

AST::BinaryOperation Parser::get_binary_operation(Syntax::Kind kind) {
  if (kind_to_binary_operation.find(kind) != kind_to_binary_operation.end()) {
    return kind_to_binary_operation.at(kind);
  } else {
    throw std::invalid_argument("Invalid binary operation");
  }
}

std::unique_ptr<AST::Type> Parser::create_type(const std::string& type) {
  if (type == "integer") {
    return std::make_unique<AST::IntegerType>();
  } else if (type == "boolean") {
    return std::make_unique<AST::BooleanType>();
  } else if (type == "void") {
    return std::make_unique<AST::VoidType>();
  } else {
    throw std::invalid_argument("Invalid type string");
  }
}

bool Parser::has_next_token() { return token_index < tokens->size(); }

Syntax::Kind Parser::peek_next_kind() {
  if (has_next_token()) {
    return tokens->at(token_index + 1).kind;
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
    current_token = &tokens->at(token_index);
  } else {
    throw std::runtime_error("No more tokens to peek");
  }
}

std::string Parser::read(Syntax::Kind kind) {
  if (has_next_token()) {
    if (tokens->at(token_index).kind == kind) {
      std::string lexeme = tokens->at(token_index).lexeme;
      go_to_next_token();
      return lexeme;
    } else {
      LOG(ERROR) << "Expected token: " << kind_to_string.at(kind);
      LOG(ERROR) << "Actual token: " << kind_to_string.at(tokens->at(token_index).kind);
      LOG(ERROR) << "line: " << tokens->at(token_index).line;
      LOG(ERROR) << "column: " << tokens->at(token_index).column;
      throw std::runtime_error("Unexpected token");
    }
  } else {
    throw std::runtime_error("No more tokens to peek");
  }
}

const std::map<Syntax::Kind, AST::BinaryOperation> Parser::kind_to_binary_operation = {
    {Syntax::Kind::kLessOrEqualOpr, AST::BinaryOperation::kLessThanOrEqual},
    {Syntax::Kind::kNotEqualOpr, AST::BinaryOperation::kNotEqual},
    {Syntax::Kind::kLessThanOpr, AST::BinaryOperation::kLessThan},
    {Syntax::Kind::kGreaterOrEqualOpr, AST::BinaryOperation::kGreaterThanOrEqual},
    {Syntax::Kind::kGreaterThanOpr, AST::BinaryOperation::kGreaterThan},
    {Syntax::Kind::kEqualToOpr, AST::BinaryOperation::kEqual},
    {Syntax::Kind::kModulusOpr, AST::BinaryOperation::kModulo},
    {Syntax::Kind::kAndOpr, AST::BinaryOperation::kAnd},
    {Syntax::Kind::kOrOpr, AST::BinaryOperation::kOr},
    {Syntax::Kind::kPlus, AST::BinaryOperation::kAdd},
    {Syntax::Kind::kMinus, AST::BinaryOperation::kSubtract},
    {Syntax::Kind::kMultiply, AST::BinaryOperation::kMultiply},
    {Syntax::Kind::kDivide, AST::BinaryOperation::kDivide},
};

const std::map<Syntax::Kind, int> Parser::kind_to_precedence = {
    {Syntax::Kind::kMultiply, 60},
    {Syntax::Kind::kDivide, 60},
    {Syntax::Kind::kModulusOpr, 60},
    {Syntax::Kind::kPlus, 50},
    {Syntax::Kind::kMinus, 50},
    {Syntax::Kind::kLessThanOpr, 40},
    {Syntax::Kind::kLessOrEqualOpr, 40},
    {Syntax::Kind::kGreaterThanOpr, 40},
    {Syntax::Kind::kGreaterOrEqualOpr, 40},
    {Syntax::Kind::kEqualToOpr, 40},
    {Syntax::Kind::kNotEqualOpr, 40},
    {Syntax::Kind::kAndOpr, 30},
    {Syntax::Kind::kOrOpr, 20},
    {Syntax::Kind::kNotOpr, 10},
};

const std::map<Syntax::Kind, std::string> Parser::kind_to_string = {
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
