#include "winzigc/visitor/codegen/codegen_visitor.h"

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

  for (const auto& arg : expression.get_arguments()) {
    std::vector<llvm::Value*> args;
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

      if (var->getType()->isPointerTy() &&
          var->getType()->getPointerElementType()->isIntegerTy(32)) {
        args.insert(args.begin(), builder->CreateGlobalStringPtr("%d"));
      } else if (var->getType()->isPointerTy() &&
                 var->getType()->getPointerElementType()->isIntegerTy(8)) {
        args.insert(args.begin(), builder->CreateGlobalStringPtr("%c"));
      } else {
        LOG(ERROR) << "Unsupported variable type";
        return nullptr;
      }

      builder->CreateCall(callee_func, args);
    } else {
      LOG(ERROR) << "'read' called with non global variable";
    }
  }
  return nullptr;
}

llvm::Value* CodeGenVisitor::codegen_output_call(const Frontend::AST::CallExpression& expression) {
  if (expression.get_arguments().size() > 1) {
    codegen_output_many_call(expression);
    return nullptr;
  }
  llvm::Function* callee_func = module->getFunction("printf");
  if (!callee_func) {
    LOG(ERROR) << "printf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;

  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return nullptr;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isIntegerTy(8)) {
      args.push_back(builder->CreateGlobalStringPtr("%c\n"));
      if (arg_type->isPointerTy()) {
        arg_val = builder->CreateLoad(arg_val);
      }
      args.push_back(arg_val);
    } else {
      args.push_back(builder->CreateGlobalStringPtr("%d\n"));
      if (arg_type->isPointerTy()) {
        arg_val = builder->CreateLoad(arg_val);
      }
      args.push_back(arg_val);
    }
    builder->CreateCall(callee_func, args);
    args.clear();
  }
  return nullptr;
}

llvm::Value*
CodeGenVisitor::codegen_output_many_call(const Frontend::AST::CallExpression& expression) {
  llvm::Function* callee_func = module->getFunction("printf");
  if (!callee_func) {
    LOG(ERROR) << "printf function not found";
    return nullptr;
  }
  std::vector<llvm::Value*> args;
  std::string format_str = "";
  std::string str_val = "";
  bool isChar = false;

  for (const auto& arg : expression.get_arguments()) {
    llvm::Value* arg_val = arg->accept(*this);
    if (arg_val == nullptr) {
      LOG(ERROR) << "Unknown argument";
      return nullptr;
    }
    llvm::Type* arg_type = arg_val->getType();
    if (arg_type->isIntegerTy(8)) {
      isChar = true;
      llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(arg_val);
      if (constInt) {
        char c = static_cast<char>(constInt->getZExtValue());
        str_val += c;
      }
    } else {
      if (arg_type->isPointerTy()) {
        arg_val = builder->CreateLoad(arg_val);
      }
      args.push_back(arg_val);
      format_str += "%d ";
    }
  }

  if (isChar) {
    format_str = "%s\n";
    llvm::Value* str_val_llvm = builder->CreateGlobalStringPtr(str_val);
    args.insert(args.begin(), str_val_llvm);
  } else {
    format_str += "\n";
  }

  llvm::Value* format_str_val = builder->CreateGlobalStringPtr(format_str);
  args.insert(args.begin(), format_str_val);

  return builder->CreateCall(callee_func, args);
}

} // namespace Visitor
} // namespace WinZigC
