#include <iostream>
#include <string>

#include "winzigc/visitor/codegen/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"

namespace WinZigC {
namespace Visitor {

void CodeGenVisitor::visit(const Frontend::AST::IntegerExpression& expression) {
  emit_location(&expression);
  llvm::Value* codegen_value =
      llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), expression.get_value());
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::BooleanExpression& expression) {
  emit_location(&expression);
  llvm::Value* codegen_value = expression.get_bool() ? llvm::ConstantInt::getTrue(*context)
                                                     : llvm::ConstantInt::getFalse(*context);
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::CharacterExpression& expression) {
  emit_location(&expression);
  llvm::Value* codegen_value =
      llvm::ConstantInt::getSigned(llvm::Type::getInt8Ty(*context), expression.get_character());
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::CallExpression& expression) {
  emit_location(&expression);
  if (expression.get_name() == "output") {
    codegen_output_call(expression);
    return;
  } else if (expression.get_name() == "read") {
    codegen_read_call(expression);
    return;
  }

  llvm::Function* callee_func = module->getFunction(expression.get_name());
  if (!callee_func) {
    LOG(ERROR) << "Unknown function referenced";
    return;
  }

  std::vector<llvm::Value*> args;
  for (const auto& arg : expression.get_arguments()) {
    arg->accept(*this);
    llvm::Value* arg_val = arg->get_codegen_value();
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isPointerTy()) {
      arg_val = builder->CreateLoad(arg_val);
    }
    args.push_back(arg_val);
  }
  llvm::Value* codegen_value = builder->CreateCall(callee_func, args);
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::IdentifierExpression& expression) {
  llvm::Value* var = lookup_variable(expression.get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return;
  }
  emit_location(&expression);
  llvm::Value* codegen_value = builder->CreateLoad(var, expression.get_name());
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::AssignmentExpression& expression) {
  llvm::Value* var = lookup_variable(expression.get_name().get_name());
  if (!var) {
    LOG(ERROR) << "Unknown variable name";
    return;
  }
  expression.get_expression().accept(*this);
  llvm::Value* value = expression.get_expression().get_codegen_value();
  emit_location(&expression);
  builder->CreateStore(value, var);
}

void CodeGenVisitor::visit(const Frontend::AST::SwapExpression& expression) {
  llvm::Value* var1 = lookup_variable(expression.get_lhs().get_name());
  llvm::Value* var2 = lookup_variable(expression.get_rhs().get_name());
  if (!var1 || !var2) {
    LOG(ERROR) << "Unknown variable name";
    return;
  }
  llvm::Value* value1 = builder->CreateLoad(var1);
  llvm::Value* value2 = builder->CreateLoad(var2);
  emit_location(&expression);
  builder->CreateStore(value2, var1);
  builder->CreateStore(value1, var2);
}

void CodeGenVisitor::visit(const Frontend::AST::IfExpression& expression) {
  emit_location(&expression);
  expression.get_condition().accept(*this);
  llvm::Value* cond = expression.get_condition().get_codegen_value();
  if (!cond) {
    LOG(ERROR) << "Unknown condition";
    return;
  }

  llvm::Function* parent_function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* then_block = llvm::BasicBlock::Create(*context, "then", parent_function);
  llvm::BasicBlock* else_block = llvm::BasicBlock::Create(*context, "else");
  llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(*context, "ifcont");
  bool add_merge_block = false;

  // condition block instructions
  builder->CreateCondBr(cond, then_block, else_block);

  // then block instructions
  builder->SetInsertPoint(then_block);
  for (const auto& statement : expression.get_then_statement()) {
    statement->accept(*this);
  }
  // final block branched from then block does not end with a terminator
  if (!builder->GetInsertBlock()->getTerminator()) {
    add_merge_block = true;
    builder->CreateBr(merge_block);
  }

  // else block instructions
  parent_function->getBasicBlockList().push_back(else_block);
  builder->SetInsertPoint(else_block);
  for (const auto& statement : expression.get_else_statement()) {
    statement->accept(*this);
  }
  // final block branched from else block does not end with a terminator
  if (!builder->GetInsertBlock()->getTerminator()) {
    add_merge_block = true;
    builder->CreateBr(merge_block);
  }

  // we need a merge block only when alleast one branch is not going to the fucntion exit block
  if (add_merge_block) {
    parent_function->getBasicBlockList().push_back(merge_block);
    builder->SetInsertPoint(merge_block);
  }
}

void CodeGenVisitor::visit(const Frontend::AST::ForExpression& expression) {
  emit_location(&expression);
  llvm::Function* function = builder->GetInsertBlock()->getParent();

  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "for_cond", function);
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "for_body");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "for_exit");

  expression.get_start_assignment().accept(*this);
  builder->CreateBr(cond_block);

  builder->SetInsertPoint(cond_block);
  expression.get_condition().accept(*this);
  llvm::Value* cond = expression.get_condition().get_codegen_value();
  builder->CreateCondBr(cond, body_block, exit_block);

  function->getBasicBlockList().push_back(body_block);
  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
  expression.get_end_assignment().accept(*this);
  builder->CreateBr(cond_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);
}

void CodeGenVisitor::visit(const Frontend::AST::RepeatUntilExpression& expression) {
  emit_location(&expression);
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "repeat_body", function);
  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "repeat_cond");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "repeat_exit");

  builder->CreateBr(body_block);

  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }

  builder->CreateBr(cond_block);
  builder->SetInsertPoint(cond_block);
  function->getBasicBlockList().push_back(cond_block);
  expression.get_condition().accept(*this);
  llvm::Value* cond = expression.get_condition().get_codegen_value();
  builder->CreateCondBr(cond, exit_block, body_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);
}

void CodeGenVisitor::visit(const Frontend::AST::WhileExpression& expression) {
  emit_location(&expression);
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* cond_block = llvm::BasicBlock::Create(*context, "while_cond", function);
  llvm::BasicBlock* body_block = llvm::BasicBlock::Create(*context, "while_body");
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "while_exit");

  builder->CreateBr(cond_block);
  builder->SetInsertPoint(cond_block);

  expression.get_condition().accept(*this);
  llvm::Value* cond = expression.get_condition().get_codegen_value();
  builder->CreateCondBr(cond, body_block, exit_block);

  function->getBasicBlockList().push_back(body_block);
  builder->SetInsertPoint(body_block);
  for (const auto& statement : expression.get_body_statements()) {
    statement->accept(*this);
  }
  builder->CreateBr(cond_block);

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);
}

void CodeGenVisitor::visit(const Frontend::AST::CaseExpression& expression) {
  emit_location(&expression);
  llvm::Function* function = builder->GetInsertBlock()->getParent();
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(*context, "case_exit");

  expression.get_expression().accept(*this);
  llvm::Value* switch_val = expression.get_expression().get_codegen_value();
  llvm::SwitchInst* switch_inst =
      builder->CreateSwitch(switch_val, exit_block, expression.get_cases().size());

  for (size_t i = 0; i < expression.get_cases().size(); i++) {
    const Frontend::AST::CaseClause& case_clause = expression.get_cases()[i];
    std::string blockName = "case_" + std::to_string(i);
    llvm::BasicBlock* case_block = llvm::BasicBlock::Create(*context, blockName, function);

    const auto& case_value = case_clause.first;
    if (std::holds_alternative<std::unique_ptr<Frontend::AST::Expression>>(case_value)) {
      Frontend::AST::Expression* value_expr =
          std::get<std::unique_ptr<Frontend::AST::Expression>>(case_value).get();
      llvm::Value* llvm_case_value = nullptr;
      if (const Frontend::AST::IdentifierExpression* const_identifier =
              dynamic_cast<const Frontend::AST::IdentifierExpression*>(value_expr)) {
        std::string case_identifier = const_identifier->get_name();
        uint32_t case_value = 0;
        if (local_user_def_type_consts.find(case_identifier) != local_user_def_type_consts.end()) {
          case_value = local_user_def_type_consts[case_identifier];
        } else if (global_user_def_type_consts.find(case_identifier) !=
                   global_user_def_type_consts.end()) {
          case_value = global_user_def_type_consts[case_identifier];
        } else {
          LOG(ERROR) << "Unknown case value";
          return;
        }
        llvm_case_value =
            llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), case_value);
      } else {
        value_expr->accept(*this);
        llvm_case_value = value_expr->get_codegen_value();
      }
      llvm::ConstantInt* const_int = llvm::dyn_cast<llvm::ConstantInt>(llvm_case_value);
      switch_inst->addCase(const_int, case_block);
    } else if (std::holds_alternative<std::pair<std::unique_ptr<Frontend::AST::Expression>,
                                                std::unique_ptr<Frontend::AST::Expression>>>(
                   case_value)) {
      auto& exprPair = std::get<std::pair<std::unique_ptr<Frontend::AST::Expression>,
                                          std::unique_ptr<Frontend::AST::Expression>>>(case_value);
      Frontend::AST::Expression* first_value_expr = exprPair.first.get();
      Frontend::AST::Expression* second_value_expr = exprPair.second.get();

      int first_value = 0;
      int second_value = 0;

      if (const Frontend::AST::IntegerExpression* first_int_expr =
              dynamic_cast<const Frontend::AST::IntegerExpression*>(first_value_expr)) {
        first_value = first_int_expr->get_value();
      }
      if (const Frontend::AST::IntegerExpression* second_int_expr =
              dynamic_cast<const Frontend::AST::IntegerExpression*>(second_value_expr)) {
        second_value = second_int_expr->get_value();
      }
      for (int i = first_value; i <= second_value; i++) {
        llvm::Value* llvm_case_value =
            llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), i);
        llvm::ConstantInt* const_int = llvm::dyn_cast<llvm::ConstantInt>(llvm_case_value);
        switch_inst->addCase(const_int, case_block);
      }
    }

    builder->SetInsertPoint(case_block);
    for (const auto& statement : case_clause.second) {
      statement->accept(*this);
    }

    if (!builder->GetInsertBlock()->getTerminator()) {
      builder->CreateBr(exit_block);
    }
  }

  function->getBasicBlockList().push_back(exit_block);
  builder->SetInsertPoint(exit_block);
  for (const auto& statement : expression.get_otherwise_clause()) {
    statement->accept(*this);
  }
}

void CodeGenVisitor::visit(const Frontend::AST::ReturnExpression& expression) {
  emit_location(&expression);
  llvm::Function* parent_function = builder->GetInsertBlock()->getParent();
  llvm::Value* return_var = lookup_variable(parent_function->getName().str());
  if (!return_var) {
    LOG(ERROR) << "Unknown return variable name";
    return;
  }
  expression.get_expression().accept(*this);
  llvm::Value* return_val = expression.get_expression().get_codegen_value();
  builder->CreateStore(return_val, return_var);
  builder->CreateBr(function_exit_block);
}

void CodeGenVisitor::visit(const Frontend::AST::BinaryExpression& expression) {
  emit_location(&expression);
  expression.get_lhs().accept(*this);
  llvm::Value* lhs = expression.get_lhs().get_codegen_value();
  expression.get_rhs().accept(*this);
  llvm::Value* rhs = expression.get_rhs().get_codegen_value();

  llvm::Value* codegen_value;
  switch (expression.get_op()) {
  case Frontend::AST::BinaryOperation::kAdd:
    codegen_value = builder->CreateAdd(lhs, rhs, "addtmp");
    break;
  case Frontend::AST::BinaryOperation::kSubtract:
    codegen_value = builder->CreateSub(lhs, rhs, "subtmp");
    break;
  case Frontend::AST::BinaryOperation::kMultiply:
    codegen_value = builder->CreateMul(lhs, rhs, "multmp");
    break;
  case Frontend::AST::BinaryOperation::kDivide:
    codegen_value = builder->CreateSDiv(lhs, rhs, "divtmp");
    break;
  case Frontend::AST::BinaryOperation::kModulo:
    codegen_value = builder->CreateSRem(lhs, rhs, "modtmp");
    break;
  case Frontend::AST::BinaryOperation::kLessThan:
    codegen_value = builder->CreateICmpSLT(lhs, rhs, "lttmp");
    break;
  case Frontend::AST::BinaryOperation::kLessThanOrEqual:
    codegen_value = builder->CreateICmpSLE(lhs, rhs, "letmp");
    break;
  case Frontend::AST::BinaryOperation::kGreaterThan:
    codegen_value = builder->CreateICmpSGT(lhs, rhs, "gttmp");
    break;
  case Frontend::AST::BinaryOperation::kGreaterThanOrEqual:
    codegen_value = builder->CreateICmpSGE(lhs, rhs, "getmp");
    break;
  case Frontend::AST::BinaryOperation::kEqual:
    codegen_value = builder->CreateICmpEQ(lhs, rhs, "eqtmp");
    break;
  case Frontend::AST::BinaryOperation::kNotEqual:
    codegen_value = builder->CreateICmpNE(lhs, rhs, "netmp");
    break;
  case Frontend::AST::BinaryOperation::kAnd:
    codegen_value = builder->CreateAnd(lhs, rhs, "andtmp");
    break;
  case Frontend::AST::BinaryOperation::kOr:
    codegen_value = builder->CreateOr(lhs, rhs, "ortmp");
    break;
  default:
    LOG(ERROR) << "Unknown binary operation";
    return;
  }
  expression.set_codegen_value(codegen_value);
}

void CodeGenVisitor::visit(const Frontend::AST::UnaryExpression& expression) {
  expression.get_expression().accept(*this);
  llvm::Value* operand = expression.get_expression().get_codegen_value();
  llvm::Value* codegen_value;
  emit_location(&expression);
  switch (expression.get_op()) {
  case Frontend::AST::UnaryOperation::kMinus:
    codegen_value = builder->CreateNeg(operand, "negtmp");
    break;
  case Frontend::AST::UnaryOperation::kPlus:
    codegen_value = operand;
    break;
  case Frontend::AST::UnaryOperation::kNot:
    codegen_value = builder->CreateNot(operand, "nottmp");
    break;
  case Frontend::AST::UnaryOperation::kSucc:
    codegen_value = builder->CreateAdd(
        operand, llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 1), "addtmp");
    break;
  case Frontend::AST::UnaryOperation::kPred:
    codegen_value = builder->CreateSub(
        operand, llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 1), "subtmp");
    break;
  default:
    LOG(ERROR) << "Unknown unary operation";
    return;
  }
  expression.set_codegen_value(codegen_value);
}

} // namespace Visitor
} // namespace WinZigC
