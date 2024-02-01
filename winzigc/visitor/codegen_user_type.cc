#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::GlobalUserTypeDef& expression) {
  for (size_t value_index = 0; value_index < expression.get_value_names().size(); value_index++) {
    llvm::Constant* const_value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context),
                                                         llvm::APInt(32, value_index, true));
    llvm::GlobalVariable* global_const = new llvm::GlobalVariable(
        *module, const_value->getType(), true, llvm::GlobalValue::InternalLinkage, const_value,
        expression.get_value_names().at(value_index));
    global_variables[llvm::StringRef(expression.get_value_names().at(value_index))] = global_const;
    global_user_def_type_consts[expression.get_value_names().at(value_index)] = value_index;
  }
  return nullptr;
}

llvm::Value* CodeGenVisitor::visit(const Frontend::AST::LocalUserTypeDef& expression) {
  for (size_t value_index = 0; value_index < expression.get_value_names().size(); value_index++) {
    llvm::Constant* const_value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context),
                                                         llvm::APInt(32, value_index, true));
    llvm::AllocaInst* alloca = builder->CreateAlloca(llvm::Type::getInt32Ty(*context), nullptr,
                                                     expression.get_value_names().at(value_index));
    builder->CreateStore(const_value, alloca);
    local_variables[llvm::StringRef(expression.get_value_names().at(value_index))] = alloca;
    local_user_def_type_consts[expression.get_value_names().at(value_index)] = value_index;
  }
  return nullptr;
}

} // namespace Visitor
} // namespace WinZigC
