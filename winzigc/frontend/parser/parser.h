#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "winzigc/frontend/syntax/kind.h"
#include "winzigc/frontend/syntax/token.h"
#include "winzigc/frontend/ast/program.h"

namespace WinZigC {
namespace Frontend {

class Parser {
public:
  Parser(std::unique_ptr<std::vector<Syntax::Token>> tokens);
  std::unique_ptr<AST::Program> parse();

private:
  std::vector<std::unique_ptr<AST::Expression>> parse_body();
  std::unique_ptr<AST::Expression> parse_expression();
  std::unique_ptr<AST::Expression> parse_primary();
  std::unique_ptr<AST::Expression> parse_binary_rhs(int precedence,
                                                    std::unique_ptr<AST::Expression> lhs);
  int get_token_precedence();
  AST::BinaryOperation get_binary_operation(Syntax::Kind kind);
  void parse_statement(std::vector<std::unique_ptr<AST::Expression>>& statements);
  void parse_assignment_statement(std::vector<std::unique_ptr<AST::Expression>>& statements);
  void parse_output_statement(std::vector<std::unique_ptr<AST::Expression>>& statements);
  std::vector<std::unique_ptr<AST::GlobalVariable>> parse_dclns();
  void parse_dcln(std::vector<std::unique_ptr<AST::GlobalVariable>>& variables);
  std::unique_ptr<AST::Type> createType(const std::string& type);
  bool has_next_token();
  Syntax::Kind peek_next_kind();
  Syntax::Token peek_next_token();
  void go_to_next_token();
  Syntax::Token get_next_token();
  std::string read(Syntax::Kind kind);

  std::unique_ptr<AST::Program> program;
  std::unique_ptr<std::vector<Syntax::Token>> tokens;
  int token_index;
  Syntax::Token* current_token;

  static const std::map<Syntax::Kind, std::string> kind_to_string;
  static const std::map<Syntax::Kind, int> kind_to_precedence;
  static const std::map<Syntax::Kind, AST::BinaryOperation> kind_to_binary_operation;
};

} // namespace Frontend
} // namespace WinZigC