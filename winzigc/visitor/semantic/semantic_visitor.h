#include <string>
#include <unordered_map>

#include "winzigc/frontend/ast/visitor.h"

#include "glog/logging.h"

namespace WinZigC {
namespace Visitor {

class SemanticError {
public:
  SemanticError(int line, int column, std::string message)
      : line(line), column(column), message(message) {}
  std::string get_error_message() {
    return ":" + std::to_string(line) + ":" + std::to_string(column) + ": " + message;
  }

private:
  int line;
  int column;
  std::string message;
};

class SemanticVisitor : public Frontend::AST::Visitor {

public:
  SemanticVisitor() = default;
  ~SemanticVisitor() = default;

  void print_errors(const std::string& program_path);
  void check(const Frontend::AST::Program& program, const std::string& program_path);

  void visit(const Frontend::AST::Program& program) override;
  void visit(const Frontend::AST::Function& function) override;

  void visit(const Frontend::AST::IntegerExpression& expression) override;
  void visit(const Frontend::AST::BooleanExpression& expression) override;
  void visit(const Frontend::AST::CharacterExpression& expression) override;

  void visit(const Frontend::AST::CallExpression& expression) override;

  void visit(const Frontend::AST::IdentifierExpression& expression) override;
  void visit(const Frontend::AST::AssignmentExpression& expression) override;
  void visit(const Frontend::AST::SwapExpression& expression) override;
  void visit(const Frontend::AST::IfExpression& expression) override;
  void visit(const Frontend::AST::ForExpression& expression) override;
  void visit(const Frontend::AST::RepeatUntilExpression& expression) override;
  void visit(const Frontend::AST::WhileExpression& expression) override;
  void visit(const Frontend::AST::CaseExpression& expression) override;
  void visit(const Frontend::AST::ReturnExpression& expression) override;
  void visit(const Frontend::AST::BinaryExpression& expression) override;
  void visit(const Frontend::AST::UnaryExpression& expression) override;

  void visit(const Frontend::AST::LocalVariable& expression) override;
  void visit(const Frontend::AST::GlobalVariable& expression) override;

  void visit(const Frontend::AST::LocalUserTypeDef& expression) override;
  void visit(const Frontend::AST::GlobalUserTypeDef& expression) override;

  void visit(const Frontend::AST::IntegerType& expression) override;
  void visit(const Frontend::AST::BooleanType& expression) override;
  void visit(const Frontend::AST::CharacterType& expression) override;
  void visit(const Frontend::AST::UserType& expression) override;

  std::string get_type(const Frontend::AST::Type& type);

private:
  std::vector<SemanticError> errors;
  std::unordered_map<std::string, std::string> global_var_to_type = {{"d", "integer"}};
  std::unordered_map<std::string, std::string> local_var_to_type;
  std::unordered_map<std::string, std::string> function_to_return_type = {{"read", "void"},
                                                                          {"output", "void"}};
  std::string current_function_return_type;
  std::string current_function_name;
  std::unordered_map<std::string, std::vector<std::string>> function_to_param_types;

  static const std::unordered_map<Frontend::AST::BinaryOperation, std::string>
      binary_op_to_token_str;
  static const std::unordered_map<Frontend::AST::UnaryOperation, std::string> unary_op_to_token_str;
};

} // namespace Visitor
} // namespace WinZigC
