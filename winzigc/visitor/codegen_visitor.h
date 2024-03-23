#pragma once

#include <map>
#include <stack>

#include "winzigc/frontend/ast/visitor.h"
#include "winzigc/frontend/ast/program.h"

#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "glog/logging.h"

namespace WinZigC {
namespace Visitor {

class CodeGenVisitor : public Frontend::AST::Visitor {
public:
  CodeGenVisitor(bool optimize = false, bool debug = false);
  ~CodeGenVisitor();

  void print_llvm_ir(std::string output_path = "") const;
  void codegen(const Frontend::AST::Program& program, std::string program_path);
  void codegen_global_user_types(
      const std::vector<std::unique_ptr<Frontend::AST::GlobalUserTypeDef>>& user_types);
  void codegen_global_vars(const Frontend::AST::Program& program);

  void codegen_func_dclns(const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions);
  llvm::FunctionType* codegen_func_dcln(const std::unique_ptr<Frontend::AST::Function>& function);
  void codegen_func_defs(const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions);
  void codegen_func_def(const std::unique_ptr<Frontend::AST::Function>& function);

  void codegen_main_body(const std::vector<std::unique_ptr<Frontend::AST::Expression>>& statements);
  void codegen_external_func_dclns();
  llvm::Value* codegen_read_call(const Frontend::AST::CallExpression& expression);
  llvm::Value* codegen_output_call(const Frontend::AST::CallExpression& expression);
  llvm::Value* codegen_output_many_call(const Frontend::AST::CallExpression& expression);

  void run_optimizations(const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions);

  llvm::Value* visit(const Frontend::AST::IntegerExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::BooleanExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::CharacterExpression& expression) override;

  llvm::Value* visit(const Frontend::AST::CallExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::IdentifierExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::AssignmentExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::SwapExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::IfExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::ForExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::RepeatUntilExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::WhileExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::CaseExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::ReturnExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::BinaryExpression& expression) override;
  llvm::Value* visit(const Frontend::AST::UnaryExpression& expression) override;

  llvm::Value* visit(const Frontend::AST::LocalVariable& expression) override;
  llvm::Value* visit(const Frontend::AST::GlobalVariable& expression) override;
  llvm::Constant* get_default_value(const Frontend::AST::Type& type);
  llvm::Value* lookup_variable(std::string var_name);

  llvm::Value* visit(const Frontend::AST::LocalUserTypeDef& expression) override;
  llvm::Value* visit(const Frontend::AST::GlobalUserTypeDef& expression) override;

  llvm::Type* visit(const Frontend::AST::IntegerType& expression) override;
  llvm::Type* visit(const Frontend::AST::BooleanType& expression) override;
  llvm::Type* visit(const Frontend::AST::CharacterType& expression) override;
  llvm::Type* visit(const Frontend::AST::UserType& expression) override;

  /* Debug Information Start */
  llvm::DISubroutineType*
  debug_create_function_type(const std::unique_ptr<Frontend::AST::Function>& function);
  llvm::DIBasicType* debug_get_type(const Frontend::AST::Type& type);

  void emit_location(const Frontend::AST::Expression* expression);
  void emit_location(const Frontend::AST::Variable* expression);
  /* Debug Information End   */

private:
  bool optimize;
  bool debug;
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<llvm::StringRef, llvm::GlobalVariable*> global_variables;
  std::map<llvm::StringRef, llvm::AllocaInst*> local_variables;
  std::map<std::string, int32_t> local_user_def_type_consts;
  std::map<std::string, int32_t> global_user_def_type_consts;
  llvm::BasicBlock* function_exit_block;

  std::unique_ptr<llvm::DIBuilder> debug_builder;
  llvm::DICompileUnit* compile_unit;
  std::stack<llvm::DIScope*> lexical_blocks;
};

} // namespace Visitor
} // namespace WinZigC
