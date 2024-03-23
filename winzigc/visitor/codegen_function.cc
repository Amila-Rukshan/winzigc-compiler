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

  /* Debug Information Start */
  llvm::DIFile* unit;
  llvm::DISubprogram* sub_program;
  unsigned line_number;
  if (debug) {
    unit = debug_builder->createFile(compile_unit->getFilename(), compile_unit->getDirectory());
    llvm::DIScope* function_context = unit;
    line_number = function->get_line();
    unsigned scope_line = line_number;
    sub_program = debug_builder->createFunction(
        function_context, function->get_name(), llvm::StringRef(), unit, line_number,
        debug_create_function_type(function), scope_line, llvm::DINode::FlagPrototyped,
        llvm::DISubprogram::SPFlagDefinition);
    llvm_function->setSubprogram(sub_program);
    lexical_blocks.push(sub_program);
    // Unset the location for the prologue emission (leading instructions with no
    // location in a function are considered part of the prologue and the debugger
    // will run past them when breaking on a function)
    builder->SetCurrentDebugLocation(llvm::DebugLoc());
  }
  /* Debug Information End   */

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
    /* Debug Information Start */
    if (debug) {
      llvm::DILocalVariable* debug_param = debug_builder->createParameterVariable(
          sub_program, function->get_parameters().at(param_index)->get_name(), param_index + 1,
          unit, line_number, debug_get_type(function->get_parameters().at(param_index)->get_type()),
          true);
      debug_builder->insertDeclare(
          alloca, debug_param, debug_builder->createExpression(),
          llvm::DILocation::get(sub_program->getContext(), line_number, 0, sub_program),
          builder->GetInsertBlock());
    }
    /* Debug Information End   */
    builder->CreateStore(&param, alloca);
  }

  for (const auto& type_def : function->get_type_defs()) {
    type_def->accept(*this);
  }

  for (const auto& local_var : function->get_local_var_dclns()) {
    local_var->accept(*this);
  }
  builder->SetCurrentDebugLocation(llvm::DebugLoc());

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
  /* Debug Information Start */
  if (debug) {
    lexical_blocks.pop();
  }
  /* Debug Information End   */
}

void CodeGenVisitor::codegen_func_defs(
    const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions) {
  for (const auto& function : functions) {
    codegen_func_def(function);
    local_variables.clear();
    local_user_def_type_consts.clear();
    function_exit_block = nullptr;
  }
}

/* Debug Information Start */
llvm::DISubroutineType* CodeGenVisitor::debug_create_function_type(
    const std::unique_ptr<Frontend::AST::Function>& function) {
  llvm::SmallVector<llvm::Metadata*, 8> element_types;
  element_types.push_back(debug_get_type(function->get_return_type()));
  for (const auto& param : function->get_parameters()) {
    element_types.push_back(debug_get_type(param->get_type()));
  }
  return debug_builder->createSubroutineType(debug_builder->getOrCreateTypeArray(element_types));
}
/* Debug Information End   */

} // namespace Visitor
} // namespace WinZigC
