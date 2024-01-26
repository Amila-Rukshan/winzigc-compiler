#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalVariable& expression) {
  llvm::Constant* default_value = nullptr;
  if (const Frontend::AST::IntegerType* integer_type =
          dynamic_cast<const Frontend::AST::IntegerType*>(&expression.get_type())) {
    default_value =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), llvm::APInt(32, 0, true));
  } else if (const Frontend::AST::BooleanType* boolean_type =
                 dynamic_cast<const Frontend::AST::BooleanType*>(&expression.get_type())) {
    default_value =
        llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), llvm::APInt(1, 0, true));
  }
  llvm::GlobalVariable* global_variable = new llvm::GlobalVariable(
      *module, default_value->getType(), false, llvm::GlobalValue::InternalLinkage, default_value,
      expression.get_name());
  global_variables[llvm::StringRef(expression.get_name())] = global_variable;
  return global_variable;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::LocalVariable& expression) {
  llvm::Type* param_type = expression.get_type().accept(*this);
  llvm::AllocaInst* alloca = builder->CreateAlloca(param_type, nullptr, expression.get_name());
  local_variables[llvm::StringRef(expression.get_name())] = alloca;
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
