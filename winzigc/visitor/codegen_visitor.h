#pragma once

#include <map>

#include "winzigc/visitor/visitor.h"
#include "winzigc/frontend/ast/program.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

namespace WinZigC {
namespace Visitor {

class CodeGenVisitor : public Visitor {
public:
  CodeGenVisitor();
  ~CodeGenVisitor();

  void print_llvm_ir() const;
  void codegen(const Frontend::AST::Program& program);
  void
  codegen_global_var_dclns(const std::vector<std::unique_ptr<Frontend::AST::GlobalVariable>>& vars);
  void codegen_main_body(const std::vector<std::unique_ptr<Frontend::AST::Expression>>& statements);
  void codegen_external_func_dclns();

  llvm::Value* visit(const Frontend::AST::IntegerExpression expression) override;
  llvm::Value* visit(const Frontend::AST::GlobalVariable expression) override;
  llvm::Value* visit(const Frontend::AST::AssignmentExpression expression) override;
  llvm::Value* visit(const Frontend::AST::BinaryExpression expression) override;

  llvm::Type* visit(const Frontend::AST::IntegerType expression) override;
  llvm::Type* visit(const Frontend::AST::BooleanType expression) override;

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::AllocaInst*> named_values;
};

} // namespace Visitor
} // namespace WinZigC
