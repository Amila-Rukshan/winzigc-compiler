#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalVariable& expression) {
  llvm::Constant* default_value = get_default_value(expression.get_type());
  llvm::GlobalVariable* global_variable = new llvm::GlobalVariable(
      *module, default_value->getType(), false, llvm::GlobalValue::InternalLinkage, default_value,
      expression.get_name());
  /* Debug Information Start */
  if (debug) {
    llvm::DIFile* unit =
        debug_builder->createFile(compile_unit->getFilename(), compile_unit->getDirectory());
    llvm::DIBasicType* basic_type = debug_get_type(expression.get_type());
    llvm::DIType* type = debug_get_type(expression.get_type());
    llvm::DIGlobalVariableExpression* var_expr = debug_builder->createGlobalVariableExpression(
        compile_unit, expression.get_name(), expression.get_name(), unit, 1, type, false);
    global_variable->addDebugInfo(var_expr);
  }
  /* Debug Information End   */
  global_variables[llvm::StringRef(expression.get_name())] = global_variable;
  return global_variable;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::LocalVariable& expression) {
  llvm::Type* param_type = expression.get_type().accept(*this);
  llvm::AllocaInst* alloca = builder->CreateAlloca(param_type, nullptr, expression.get_name());
  llvm::Constant* default_value = get_default_value(expression.get_type());
  builder->CreateStore(default_value, alloca);
  /* Debug Information Start */
  if (debug) {
    llvm::DIFile* unit =
        debug_builder->createFile(compile_unit->getFilename(), compile_unit->getDirectory());
    llvm::DIBasicType* basic_type = debug_get_type(expression.get_type());
    llvm::DIType* type = debug_get_type(expression.get_type());
    llvm::DILocalVariable* var = debug_builder->createAutoVariable(
        lexical_blocks.top(), expression.get_name(), unit, 1, type);
    debug_builder->insertDeclare(
        alloca, var, debug_builder->createExpression(),
        llvm::DebugLoc::get(expression.get_line(), 0, lexical_blocks.top()),
        builder->GetInsertBlock());
  }
  /* Debug Information End   */

  local_variables[llvm::StringRef(expression.get_name())] = alloca;
  return nullptr;
}

llvm::Constant* CodeGenVisitor::get_default_value(const Frontend::AST::Type& type) {
  if (const Frontend::AST::IntegerType* integer_type =
          dynamic_cast<const Frontend::AST::IntegerType*>(&type))
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), llvm::APInt(32, 0, true));
  if (const Frontend::AST::BooleanType* boolean_type =
          dynamic_cast<const Frontend::AST::BooleanType*>(&type))
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), llvm::APInt(1, 0, true));
  if (const Frontend::AST::CharacterType* char_type =
          dynamic_cast<const Frontend::AST::CharacterType*>(&type))
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), llvm::APInt(8, 0, true));
  if (const Frontend::AST::UserType* integer_type =
          dynamic_cast<const Frontend::AST::UserType*>(&type))
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), llvm::APInt(32, 0, true));

  LOG(ERROR) << "Unable to generate default value for unknown type";
  return nullptr;
}

llvm::Value* CodeGenVisitor::lookup_variable(std::string var_name) {
  if (local_variables.find(llvm::StringRef(var_name)) != local_variables.end()) {
    return local_variables[llvm::StringRef(var_name)];
  } else if (global_variables.find(llvm::StringRef(var_name)) != global_variables.end()) {
    return global_variables[llvm::StringRef(var_name)];
  }
  LOG(ERROR) << "Unknown variable: " << var_name;
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
