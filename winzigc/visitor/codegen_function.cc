#include "winzigc/visitor/codegen_visitor.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::FunctionType*
CodeGenVisitor::codegen_func_dcln(const std::unique_ptr<Frontend::AST::Function>& function) {
  std::vector<llvm::Type*> param_types;
  for (const auto& param : function->get_parameters()) {
    param_types.push_back(param->get_type().accept(*this));
  }
  llvm::Type* return_type = function->get_return_type().accept(*this);
  return llvm::FunctionType::get(return_type, param_types, false);
}

void CodeGenVisitor::codegen_func_dclns(
    const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions) {
  for (const auto& function : functions) {
    llvm::FunctionType* func_type = codegen_func_dcln(function);
    llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, function->get_name(),
                           module.get());
  }
}

void CodeGenVisitor::codegen_func_def(const std::unique_ptr<Frontend::AST::Function>& function) {
  llvm::Function* llvm_function = module->getFunction(llvm::StringRef(function->get_name()));
  llvm::BasicBlock* function_entry_block =
      llvm::BasicBlock::Create(*context, "entry", llvm_function);
  function_exit_block = llvm::BasicBlock::Create(*context, "exit");
  builder->SetInsertPoint(function_entry_block);

  llvm::Type* return_type = function->get_return_type().accept(*this);
  llvm::AllocaInst* return_alloca =
      builder->CreateAlloca(return_type, nullptr, function->get_name());
  builder->CreateStore(llvm::ConstantInt::get(return_type, 0), return_alloca);
  local_variables[llvm::StringRef(function->get_name())] = return_alloca;

  for (auto& param : llvm_function->args()) {
    int param_index = param.getArgNo();
    llvm::Type* param_type = llvm_function->getFunctionType()->getParamType(param_index);
    llvm::AllocaInst* alloca = builder->CreateAlloca(
        param_type, nullptr, function->get_parameters().at(param_index)->get_name());
    local_variables[llvm::StringRef(function->get_parameters().at(param_index)->get_name())] =
        alloca;
    builder->CreateStore(&param, alloca);
  }

  for (const auto& local_var : function->get_local_var_dclns()) {
    local_var->accept(*this);
  }

  for (const auto& expr : function->get_function_body_exprs()) {
    expr->accept(*this);
  }

  if (!builder->GetInsertBlock()->getTerminator()) {
    builder->CreateBr(function_exit_block);
  }

  llvm_function->getBasicBlockList().push_back(function_exit_block);
  builder->SetInsertPoint(function_exit_block);
  if (llvm_function->getReturnType()->isVoidTy()) {
    builder->CreateRetVoid();
  } else {
    llvm::Value* return_val = builder->CreateLoad(return_alloca);
    builder->CreateRet(return_val);
  }
}

void CodeGenVisitor::codegen_func_defs(
    const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions) {
  for (const auto& function : functions) {
    codegen_func_def(function);
    local_variables.clear();
    function_exit_block = nullptr;
  }
}

} // namespace Visitor
} // namespace WinZigC
