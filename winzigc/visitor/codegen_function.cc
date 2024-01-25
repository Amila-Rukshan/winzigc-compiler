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
  llvm::BasicBlock* func_entry_block = llvm::BasicBlock::Create(*context, "entry", llvm_function);
  builder->SetInsertPoint(func_entry_block);

  local_variables.clear();
  for (auto& param : llvm_function->args()) {
    int param_index = param.getArgNo();
    std::string param_name = function->get_parameters().at(param_index)->get_name();
    llvm::Type* param_type = llvm_function->getFunctionType()->getParamType(param_index);
    llvm::AllocaInst* alloca = builder->CreateAlloca(param_type, nullptr, param_name);
    local_variables[llvm::StringRef(param_name)] = alloca;
    builder->CreateStore(&param, alloca);
  }

  // TODO: create a common exit block which can be used by all control flows to jump with return
  // values
  llvm::Value* return_value;
  for (const auto& expr : function->get_function_body_exprs()) {
    return_value = expr->accept(*this);
  }

  if (llvm_function->getReturnType()->isVoidTy()) {
    builder->CreateRetVoid();
  } else {
    // builder->CreateRet(return_value);
  }
  // TODO: fianlly insert it at the end of the function
}

void CodeGenVisitor::codegen_func_defs(
    const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions) {
  for (const auto& function : functions) {
    codegen_func_def(function);
  }
}

} // namespace Visitor
} // namespace WinZigC
