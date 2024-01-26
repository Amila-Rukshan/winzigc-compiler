#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"

namespace WinZigC {
namespace Visitor {

llvm::Value* CodeGenVisitor::codegen_read_call(const Frontend::AST::CallExpression& expression) {
  llvm::Function* callee_func = module->getFunction("scanf");
  if (!callee_func) {
    LOG(ERROR) << "scanf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;
  args.push_back(builder->CreateGlobalStringPtr("%d"));

  for (const auto& arg : expression.get_arguments()) {
    if (const Frontend::AST::IdentifierExpression* var_identifier =
            dynamic_cast<const Frontend::AST::IdentifierExpression*>(arg.get())) {
      llvm::Value* var = lookup_variable(var_identifier->get_name());
      if (!var) {
        LOG(ERROR) << "Unknown variable name";
        return nullptr;
      }
      llvm::ConstantInt* zero = llvm::ConstantInt::getSigned(llvm::Type::getInt64Ty(*context), 0);
      llvm::Value* i_ptr = builder->CreateInBoundsGEP(var, zero);
      args.push_back(i_ptr);
    } else {
      LOG(ERROR) << "'read' called with non global variable";
    }
  }
  return builder->CreateCall(callee_func, args);
}

llvm::Value* CodeGenVisitor::codegen_output_call(const Frontend::AST::CallExpression& expression) {
  llvm::Function* callee_func = module->getFunction("printf");
  if (!callee_func) {
    LOG(ERROR) << "printf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;
  std::string format_str = "%d";
  for (int i = 1; i < expression.get_arguments().size(); ++i) {
    format_str += " %d";
  }
  format_str += "\n";
  args.push_back(builder->CreateGlobalStringPtr(format_str));

  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return nullptr;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isPointerTy()) {
      arg_val = builder->CreateLoad(arg_val);
    }
    args.push_back(arg_val);
  }
  return builder->CreateCall(callee_func, args);
}

} // namespace Visitor
} // namespace WinZigC
