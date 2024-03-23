#include <filesystem>

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
#include "llvm/Transforms/Scalar/DCE.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

namespace WinZigC {
namespace Visitor {

CodeGenVisitor::CodeGenVisitor(bool optimize, bool debug)
    : optimize(optimize), debug(debug), context(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}

CodeGenVisitor::~CodeGenVisitor() {}

void CodeGenVisitor::print_llvm_ir(std::string output_path) const {
  std::error_code error;
  llvm::raw_fd_ostream dest(output_path + ".ll", error);

  if (error) {
    LOG(ERROR) << "Could not open file: " << error.message();
    return;
  }

  module->print(dest, nullptr);
}

void CodeGenVisitor::codegen(const Frontend::AST::Program& program, std::string program_path) {
  module = std::make_unique<llvm::Module>(program.get_name(), *context);
  /* Debug Information Start */
  if (debug) {
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                          llvm::DEBUG_METADATA_VERSION);
    // Darwin only supports dwarf2
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 2);
    debug_builder = std::make_unique<llvm::DIBuilder>(*module);

    std::filesystem::path path(program_path);
    std::string directory = path.parent_path().string();
    std::string filename = path.filename().string();

    compile_unit = debug_builder->createCompileUnit(llvm::dwarf::DW_LANG_C,
                                                    debug_builder->createFile(filename, directory),
                                                    "WinZigC Compiler", false, "", 0);
  }
  /* Debug Information End   */

  codegen_external_func_dclns();
  codegen_global_user_types(program.get_user_types());
  codegen_global_vars(program);
  codegen_func_dclns(program.get_functions());
  codegen_func_defs(program.get_functions());
  codegen_main_body(program.get_statements());
  if (optimize) {
    run_optimizations(program.get_functions());
  }

  /* Debug Information Start */
  if (debug) {
    debug_builder->finalize();
  }
  /* Debug Information End   */
}

void CodeGenVisitor::codegen_global_user_types(
    const std::vector<std::unique_ptr<Frontend::AST::GlobalUserTypeDef>>& user_types) {
  for (const auto& user_type : user_types) {
    user_type->accept(*this);
  }
}

void CodeGenVisitor::codegen_global_vars(const Frontend::AST::Program& program) {
  program.get_discard_variable()->accept(*this);
  for (const auto& var : program.get_variables()) {
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

  /* Debug Information Start */
  if (debug) {
    llvm::DIFile* unit =
        debug_builder->createFile(compile_unit->getFilename(), compile_unit->getDirectory());
    llvm::DIScope* function_context = unit;
    unsigned line_number = statements[0]->get_line();
    unsigned scope_line = line_number;

    llvm::SmallVector<llvm::Metadata*, 8> element_types;
    element_types.push_back(
        debug_builder->createBasicType("integer", 32, llvm::dwarf::DW_ATE_signed));
    llvm::DISubroutineType* debug_main_func_type =
        debug_builder->createSubroutineType(debug_builder->getOrCreateTypeArray(element_types));

    llvm::DISubprogram* sub_program = debug_builder->createFunction(
        function_context, "main", llvm::StringRef(), unit, line_number, debug_main_func_type,
        scope_line, llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);
    main_func->setSubprogram(sub_program);
    lexical_blocks.push(sub_program);
    builder->SetCurrentDebugLocation(llvm::DebugLoc());
  }
  /* Debug Information End   */

  for (const auto& statement : statements) {
    statement->accept(*this);
  }

  /* Debug Information Start */
  if (debug) {
    lexical_blocks.pop();
  }
  /* Debug Information End   */
  builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0));
}

void CodeGenVisitor::codegen_external_func_dclns() {
  module->getOrInsertFunction(
      "printf", llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*context),
                                        llvm::Type::getInt8Ty(*context)->getPointerTo(), true));
  module->getOrInsertFunction(
      "scanf", llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*context),
                                       llvm::Type::getInt8Ty(*context)->getPointerTo(), true));
}

void CodeGenVisitor::run_optimizations(
    const std::vector<std::unique_ptr<Frontend::AST::Function>>& functions) {
  llvm::legacy::FunctionPassManager fpm(module.get());
  fpm.add(llvm::createDeadCodeEliminationPass());
  fpm.add(llvm::createInstructionCombiningPass());
  fpm.add(llvm::createReassociatePass());
  fpm.add(llvm::createGVNPass());
  fpm.add(llvm::createCFGSimplificationPass());
  fpm.doInitialization();
  for (const auto& function : functions) {
    fpm.run(*module->getFunction(function->get_name()));
  }
  llvm::Function* main_function = module->getFunction(llvm::StringRef("main"));
  fpm.run(*main_function);
}

/* Debug Information Start */
llvm::DIBasicType* CodeGenVisitor::debug_get_type(const Frontend::AST::Type& type) {
  if (const Frontend::AST::IntegerType* integer_type =
          dynamic_cast<const Frontend::AST::IntegerType*>(&type))
    return debug_builder->createBasicType("integer", 32, llvm::dwarf::DW_ATE_signed);
  if (const Frontend::AST::BooleanType* boolean_type =
          dynamic_cast<const Frontend::AST::BooleanType*>(&type))
    return debug_builder->createBasicType("boolean", 8, llvm::dwarf::DW_ATE_boolean);
  if (const Frontend::AST::CharacterType* char_type =
          dynamic_cast<const Frontend::AST::CharacterType*>(&type))
    return debug_builder->createBasicType("char", 8, llvm::dwarf::DW_ATE_signed_char);
  // otherwise, assume it is a user type
  return debug_builder->createBasicType("integer", 32, llvm::dwarf::DW_ATE_signed);
}

void CodeGenVisitor::emit_location(const Frontend::AST::Expression* expression) {
  if (!debug)
    return;
  if (expression == nullptr)
    return builder->SetCurrentDebugLocation(llvm::DebugLoc());
  llvm::DIScope* scope = !lexical_blocks.empty() ? lexical_blocks.top() : compile_unit;
  builder->SetCurrentDebugLocation(llvm::DILocation::get(
      scope->getContext(), expression->get_line(), expression->get_column(), scope));
}

void CodeGenVisitor::emit_location(const Frontend::AST::Variable* expression) {
  if (!debug)
    return;
  if (expression == nullptr)
    return builder->SetCurrentDebugLocation(llvm::DebugLoc());
  llvm::DIScope* scope = !lexical_blocks.empty() ? lexical_blocks.top() : compile_unit;
  builder->SetCurrentDebugLocation(llvm::DILocation::get(
      scope->getContext(), expression->get_line(), expression->get_column(), scope));
}
/* Debug Information End   */

} // namespace Visitor
} // namespace WinZigC
