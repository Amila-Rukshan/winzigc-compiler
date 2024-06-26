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
  void parse_body(std::vector<std::unique_ptr<AST::Expression>>& statements);
  std::unique_ptr<AST::Expression> parse_expression();
  std::unique_ptr<AST::Expression> parse_primary();
  std::unique_ptr<AST::Expression> parse_binary_rhs(int precedence,
                                                    std::unique_ptr<AST::Expression> lhs);
  int get_token_precedence();
  AST::BinaryOperation get_binary_operation(Syntax::Kind kind);
  void parse_statement(std::vector<std::unique_ptr<AST::Expression>>& statements);

  std::unique_ptr<AST::Expression> parse_assignment_statement();
  std::unique_ptr<AST::Expression> parse_output_statement();
  std::unique_ptr<AST::Expression> parse_read_statement();
  std::unique_ptr<AST::Expression> parse_return_statement();
  std::unique_ptr<AST::Expression> parse_if_statement();
  std::unique_ptr<AST::Expression> parse_for_statement();
  std::unique_ptr<AST::Expression> parse_repeat_until_statement();
  std::unique_ptr<AST::Expression> parse_while_statement();

  std::unique_ptr<AST::Expression> parse_case_statement();
  void parse_case_clauses(std::vector<AST::CaseClause>& case_clauses);
  AST::CaseClause parse_case_clause();
  AST::CaseValue parse_case_value();
  void parse_otherwise_clause(std::vector<std::unique_ptr<AST::Expression>>& otherwise_statements);
  std::unique_ptr<AST::Expression> parse_const_value();

  std::vector<std::unique_ptr<AST::GlobalVariable>> parse_global_dclns();
  void parse_global_dcln(std::vector<std::unique_ptr<AST::GlobalVariable>>& variables);
  std::vector<std::unique_ptr<AST::LocalVariable>> parse_local_dclns();
  void parse_local_dcln(std::vector<std::unique_ptr<AST::LocalVariable>>& variables);

  std::vector<std::unique_ptr<AST::GlobalUserTypeDef>> parse_global_user_type_defs();
  void parse_global_user_type_def(std::vector<std::unique_ptr<AST::GlobalUserTypeDef>>& type_defs);
  std::vector<std::unique_ptr<AST::LocalUserTypeDef>> parse_local_user_type_defs();
  void parse_local_user_type_def(std::vector<std::unique_ptr<AST::LocalUserTypeDef>>& type_defs);
  std::vector<std::string> parse_literal_list();

  std::vector<std::unique_ptr<AST::Function>> parse_functions();
  void parse_function(std::vector<std::unique_ptr<AST::Function>>& functions);
  void parse_params(std::vector<std::unique_ptr<AST::LocalVariable>>& params);

  std::unique_ptr<AST::Type> create_type(const std::string& type);
  bool has_next_token();
  Syntax::Kind peek_next_kind();
  Syntax::Token peek_next_token();
  void go_to_next_token();
  Syntax::Token get_next_token();
  std::string read(Syntax::Kind kind);
  bool get_bool(std::string lexeme);

  std::unique_ptr<AST::Program> program;
  std::unique_ptr<std::vector<Syntax::Token>> tokens;
  int token_index;
  Syntax::Token* current_token;
  std::vector<std::string> global_user_types;
  std::vector<std::string> local_user_types;

  static const std::map<Syntax::Kind, std::string> kind_to_string;
  static const std::map<Syntax::Kind, int> kind_to_precedence;
  static const std::map<Syntax::Kind, AST::BinaryOperation> kind_to_binary_operation;
};

} // namespace Frontend
} // namespace WinZigC
