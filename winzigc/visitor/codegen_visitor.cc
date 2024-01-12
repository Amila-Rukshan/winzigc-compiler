#include "winzigc/visitor/codegen_visitor.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

namespace WinZigC {
namespace Visitor {

CodeGenVisitor::CodeGenVisitor()
    : context(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}

CodeGenVisitor::~CodeGenVisitor() {}

void CodeGenVisitor::print_llvm_ir() const { module->print(llvm::errs(), nullptr); }

void CodeGenVisitor::codegen(const Frontend::AST::Program& program) {
  module = std::make_unique<llvm::Module>(program.get_name(), *context);
  codegen_external_func_dclns();
  codegen_global_var_dclns(program.get_variables());
  codegen_main_body(program.get_statements());
}

void CodeGenVisitor::codegen_global_var_dclns(
    const std::vector<std::unique_ptr<Frontend::AST::GlobalVariable>>& vars) {
  for (const auto& var : vars) {
    var->accept(*this);
  }
}

void CodeGenVisitor::codegen_main_body(
    const std::vector<std::unique_ptr<Frontend::AST::Expression>>& statements) {
  llvm::FunctionType* func_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);
  llvm::Function* main_func =
      llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, "main", module.get());
  llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(*context, "entry", main_func);
  builder->SetInsertPoint(entry_block);

  for (const auto& statement : statements) {
    statement->accept(*this);
  }

  builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0));
}

void CodeGenVisitor::codegen_external_func_dclns() {
  module->getOrInsertFunction(
      "printf", llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*context),
                                        llvm::Type::getInt8Ty(*context)->getPointerTo(), true));
}

} // namespace Visitor
} // namespace WinZigC
