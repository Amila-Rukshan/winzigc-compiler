#include "winzigc/visitor/codegen_visitor.h"

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
      *module, default_value->getType(), false, llvm::GlobalValue::InternalLinkage,
      default_value, expression.get_name());
  return global_variable;
}

} // namespace Visitor
} // namespace WinZigC
