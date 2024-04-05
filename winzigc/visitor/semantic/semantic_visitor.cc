#include <filesystem>

#include "winzigc/visitor/semantic/semantic_visitor.h"

#include "glog/logging.h"

namespace WinZigC {
namespace Visitor {

void SemanticVisitor::print_errors(const std::string& program_path) {
  std::filesystem::path path(program_path);
  std::string directory = path.parent_path().string();
  std::string filename = path.filename().string();
  for (auto& error : errors) {
    LOG(ERROR) << filename << error.get_error_message();
  }
  if (errors.size() > 0) {
    LOG(ERROR) << "WinZigC compiler exiting...";
    exit(1);
  }
}

void SemanticVisitor::check(const Frontend::AST::Program& program,
                            const std::string& program_path) {
  program.accept(*this);
  print_errors(program_path);
}

void SemanticVisitor::visit(const Frontend::AST::Program& program) {
  for (const auto& user_type : program.get_user_types()) {
    user_type->accept(*this);
  }
  for (const auto& global_var : program.get_variables()) {
    global_var->accept(*this);
  }
  for (const auto& function : program.get_functions()) {
    function->accept(*this);
  }
  for (const auto& statement : program.get_statements()) {
    statement->accept(*this);
  }
};

void SemanticVisitor::visit(const Frontend::AST::Function& function) {
  current_function_name = function.get_name();
  current_function_return_type = get_type(function.get_return_type());
  local_var_to_type.insert({current_function_name, current_function_return_type});
  function_to_return_type.insert({current_function_name, current_function_return_type});
  std::vector<std::string> param_types;
  for (const auto& param : function.get_parameters()) {
    param->accept(*this);
    param_types.push_back(get_type(param->get_type()));
  }
  function_to_param_types.insert({current_function_name, param_types});
  for (const auto& user_type : function.get_type_defs()) {
    user_type->accept(*this);
  }
  for (const auto& local_var : function.get_local_var_dclns()) {
    local_var->accept(*this);
  }
  for (const auto& statement : function.get_function_body_exprs()) {
    statement->accept(*this);
  }
  local_var_to_type.clear();
};

void SemanticVisitor::visit(const Frontend::AST::IntegerExpression& expression) {
  expression.set_type_info("integer");
};

void SemanticVisitor::visit(const Frontend::AST::BooleanExpression& expression) {
  expression.set_type_info("boolean");
};

void SemanticVisitor::visit(const Frontend::AST::CharacterExpression& expression) {
  expression.set_type_info("char");
};

void SemanticVisitor::visit(const Frontend::AST::CallExpression& expression) {
  expression.set_type_info(function_to_return_type[expression.get_name()]);
  if (expression.get_name() == "read" || expression.get_name() == "output") {
    return;
  }
  if (function_to_return_type.find(expression.get_name()) == function_to_return_type.end()) {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "Undeclared function: '" + expression.get_name() + "'"));
    return;
  }
  if (function_to_param_types[expression.get_name()].size() != expression.get_arguments().size()) {
    errors.push_back(
        SemanticError(expression.get_line(), expression.get_column(),
                      "Function call argument count mismatch: '" + expression.get_name() + "'"));
    return;
  }
  for (const auto& argument : expression.get_arguments()) {
    argument->accept(*this);
  }
  for (size_t i = 0; i < expression.get_arguments().size(); ++i) {
    if (expression.get_arguments()[i]->get_type_info() !=
        function_to_param_types[expression.get_name()][i]) {
      errors.push_back(SemanticError(
          expression.get_arguments()[i]->get_line(), expression.get_arguments()[i]->get_column(),
          "Function call argument type mismatch, Expected: '" +
              function_to_param_types[expression.get_name()][i] + "', Found: '" +
              expression.get_arguments()[i]->get_type_info() + "' in " + expression.get_name()));
    }
  }
};

void SemanticVisitor::visit(const Frontend::AST::IdentifierExpression& expression) {
  std::string variable_type;
  if (local_var_to_type.find(expression.get_name()) != local_var_to_type.end()) {
    variable_type = local_var_to_type[expression.get_name()];
  } else if (global_var_to_type.find(expression.get_name()) != global_var_to_type.end()) {
    variable_type = global_var_to_type[expression.get_name()];
  } else {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "Undeclared variable: '" + expression.get_name() + "'"));
  }
  expression.set_type_info(variable_type);
};

void SemanticVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  expression.get_name().accept(*this);
  expression.get_expression().accept(*this);
  if (expression.get_name().get_type_info() != expression.get_expression().get_type_info()) {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "Assignment type mismatch: '" +
                                       expression.get_name().get_type_info() + "' and '" +
                                       expression.get_expression().get_type_info() + "'"));
  }
};

void SemanticVisitor::visit(const Frontend::AST::SwapExpression& expression) {
  expression.get_lhs().accept(*this);
  expression.get_rhs().accept(*this);
  if (expression.get_lhs().get_type_info() != expression.get_rhs().get_type_info()) {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "Swap type mismatch: '" + expression.get_lhs().get_type_info() +
                                       "' and '" + expression.get_rhs().get_type_info() + "'"));
  }
};

void SemanticVisitor::visit(const Frontend::AST::IfExpression& expression) {
  expression.get_condition().accept(*this);
  if (expression.get_condition().get_type_info() != "boolean") {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "If condition type should be boolean"));
  }
  for (const auto& then_statement : expression.get_then_statement()) {
    then_statement->accept(*this);
  }
  for (const auto& else_statement : expression.get_else_statement()) {
    else_statement->accept(*this);
  }
};

void SemanticVisitor::visit(const Frontend::AST::ForExpression& expression) {
  expression.get_start_assignment().accept(*this);
  expression.get_condition().accept(*this);
  if (expression.get_condition().get_type_info() != "boolean") {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "For condition type should be boolean"));
  }
  expression.get_end_assignment().accept(*this);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
};

void SemanticVisitor::visit(const Frontend::AST::RepeatUntilExpression& expression) {
  expression.get_condition().accept(*this);
  if (expression.get_condition().get_type_info() != "boolean") {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "Repeat until condition type should be boolean"));
  }
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
};

void SemanticVisitor::visit(const Frontend::AST::WhileExpression& expression) {
  expression.get_condition().accept(*this);
  if (expression.get_condition().get_type_info() != "boolean") {
    errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                   "While condition type should be boolean"));
  }
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
};

void SemanticVisitor::visit(const Frontend::AST::CaseExpression& expression){
    // TODO: Implement this
};

void SemanticVisitor::visit(const Frontend::AST::ReturnExpression& expression) {
  expression.get_expression().accept(*this);
  if (expression.get_expression().get_type_info() != current_function_return_type) {
    // include current function name in the error message
    errors.push_back(SemanticError(
        expression.get_line(), expression.get_column(),
        "Return type mismatch: '" + expression.get_expression().get_type_info() + "' and '" +
            current_function_return_type + "' in " + current_function_name));
  }
};

void SemanticVisitor::visit(const Frontend::AST::BinaryExpression& expression) {
  expression.get_lhs().accept(*this);
  expression.get_rhs().accept(*this);
  switch (expression.get_op()) {
  case Frontend::AST::BinaryOperation::kAdd:
  case Frontend::AST::BinaryOperation::kSubtract:
  case Frontend::AST::BinaryOperation::kMultiply:
  case Frontend::AST::BinaryOperation::kDivide:
  case Frontend::AST::BinaryOperation::kModulo:
    expression.set_type_info("integer");
    if (expression.get_lhs().get_type_info() != "integer" ||
        expression.get_rhs().get_type_info() != "integer") {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Binary operator '" +
                                         binary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to integer type"));
    }
    break;
  case Frontend::AST::BinaryOperation::kLessThan:
  case Frontend::AST::BinaryOperation::kLessThanOrEqual:
  case Frontend::AST::BinaryOperation::kGreaterThan:
  case Frontend::AST::BinaryOperation::kGreaterThanOrEqual:
    expression.set_type_info("boolean");
    if (!(expression.get_lhs().get_type_info() == expression.get_rhs().get_type_info() &&
          (expression.get_lhs().get_type_info() == "integer" ||
           expression.get_lhs().get_type_info() == "char"))) {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Binary operator '" +
                                         binary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to integer or char type"));
    }
    break;
  case Frontend::AST::BinaryOperation::kEqual:
  case Frontend::AST::BinaryOperation::kNotEqual:
    expression.set_type_info("boolean");
    if (expression.get_lhs().get_type_info() != expression.get_rhs().get_type_info()) {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Binary operator '" +
                                         binary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to same type"));
    }
    break;
  case Frontend::AST::BinaryOperation::kAnd:
  case Frontend::AST::BinaryOperation::kOr:
    expression.set_type_info("boolean");
    if (expression.get_lhs().get_type_info() != "boolean" ||
        expression.get_rhs().get_type_info() != "boolean") {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Binary operator '" +
                                         binary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to boolean type"));
    }
    break;
  }
};

void SemanticVisitor::visit(const Frontend::AST::UnaryExpression& expression) {
  expression.get_expression().accept(*this);
  switch (expression.get_op()) {
  case Frontend::AST::UnaryOperation::kMinus:
  case Frontend::AST::UnaryOperation::kPlus:
  case Frontend::AST::UnaryOperation::kSucc:
  case Frontend::AST::UnaryOperation::kPred:
    expression.set_type_info("integer");
    if (expression.get_expression().get_type_info() != "integer") {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Unary operator '" +
                                         unary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to integer type"));
    }
    break;
  case Frontend::AST::UnaryOperation::kNot:
    expression.set_type_info("boolean");
    if (expression.get_expression().get_type_info() != "boolean") {
      errors.push_back(SemanticError(expression.get_line(), expression.get_column(),
                                     "Unary operator '" +
                                         unary_op_to_token_str.at(expression.get_op()) +
                                         "' can only be applied to boolean type"));
    }
    break;
  }
};

void SemanticVisitor::visit(const Frontend::AST::LocalVariable& expression) {
  if (local_var_to_type.find(expression.get_name()) != local_var_to_type.end()) {
    errors.push_back(
        SemanticError(expression.get_line(), expression.get_column(),
                      "Redeclaration of local variable: '" + expression.get_name() + "'"));
  }
  local_var_to_type[expression.get_name()] = get_type(expression.get_type());
};

void SemanticVisitor::visit(const Frontend::AST::GlobalVariable& expression) {
  if (expression.get_name() == "d")
    return;
  auto var_type = global_var_to_type.find(expression.get_name());
  if (var_type != global_var_to_type.end()) {
    errors.push_back(
        SemanticError(expression.get_line(), expression.get_column(),
                      "Redeclaration of global variable: '" + expression.get_name() + "'"));
  }
  global_var_to_type[expression.get_name()] = get_type(expression.get_type());
};

void SemanticVisitor::visit(const Frontend::AST::LocalUserTypeDef& expression) {
  for (const auto& user_value : expression.get_value_names()) {
    local_var_to_type[user_value] = "integer";
  }
};
void SemanticVisitor::visit(const Frontend::AST::GlobalUserTypeDef& expression) {
  for (const auto& user_value : expression.get_value_names()) {
    global_var_to_type[user_value] = "integer";
  }
};

void SemanticVisitor::visit(const Frontend::AST::IntegerType& expression){};
void SemanticVisitor::visit(const Frontend::AST::BooleanType& expression){};
void SemanticVisitor::visit(const Frontend::AST::CharacterType& expression){};
void SemanticVisitor::visit(const Frontend::AST::UserType& expression){};

std::string SemanticVisitor::get_type(const Frontend::AST::Type& type) {
  if (const Frontend::AST::IntegerType* integer_type =
          dynamic_cast<const Frontend::AST::IntegerType*>(&type)) {
    return "integer";
  } else if (const Frontend::AST::BooleanType* boolean_type =
                 dynamic_cast<const Frontend::AST::BooleanType*>(&type)) {
    return "boolean";
  } else if (const Frontend::AST::CharacterType* char_type =
                 dynamic_cast<const Frontend::AST::CharacterType*>(&type)) {
    return "char";
  } else if (const Frontend::AST::UserType* user_type =
                 dynamic_cast<const Frontend::AST::UserType*>(&type)) {
    return "integer";
  }
  return "";
}

const std::unordered_map<Frontend::AST::BinaryOperation, std::string>
    SemanticVisitor::binary_op_to_token_str = {
        {Frontend::AST::BinaryOperation::kAdd, "+"},
        {Frontend::AST::BinaryOperation::kSubtract, "-"},
        {Frontend::AST::BinaryOperation::kMultiply, "*"},
        {Frontend::AST::BinaryOperation::kDivide, "/"},
        {Frontend::AST::BinaryOperation::kModulo, "mod"},
        {Frontend::AST::BinaryOperation::kLessThan, "<"},
        {Frontend::AST::BinaryOperation::kLessThanOrEqual, "<="},
        {Frontend::AST::BinaryOperation::kGreaterThan, ">"},
        {Frontend::AST::BinaryOperation::kGreaterThanOrEqual, ">="},
        {Frontend::AST::BinaryOperation::kEqual, "="},
        {Frontend::AST::BinaryOperation::kNotEqual, "<>"},
        {Frontend::AST::BinaryOperation::kAnd, "and"},
        {Frontend::AST::BinaryOperation::kOr, "or"},
};

const std::unordered_map<Frontend::AST::UnaryOperation, std::string>
    SemanticVisitor::unary_op_to_token_str = {
        {Frontend::AST::UnaryOperation::kMinus, "-"},
        {Frontend::AST::UnaryOperation::kPlus, "+"},
        {Frontend::AST::UnaryOperation::kSucc, "succ(..)"},
        {Frontend::AST::UnaryOperation::kPred, "pred(..)"},
        {Frontend::AST::UnaryOperation::kNot, "not"},
};

} // namespace Visitor
} // namespace WinZigC
