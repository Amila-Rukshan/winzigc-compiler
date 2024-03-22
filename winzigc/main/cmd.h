#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "winzigc/frontend/lexer/lexer.h"
#include "winzigc/frontend/parser/parser.h"
#include "winzigc/frontend/ast/program.h"
#include "winzigc/visitor/codegen_visitor.h"

#include "glog/logging.h"

namespace WinZigC {

void init_logger_once(char* program_name) {
  static bool is_logging_initialized = false;
  if (!is_logging_initialized) {
    google::InitGoogleLogging(program_name);
    is_logging_initialized = true;
  }
}

int main(int argc, char** argv) {
  init_logger_once(argv[0]);

  bool optimize = false;
  bool debug = false;
  std::string program_path;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-opt") {
      optimize = true;
    } else if (arg == "-dbg") {
      debug = true;
    } else {
      program_path = arg;
    }
  }

  if (program_path.empty()) {
    LOG(ERROR) << "Please provide a file path.";
    return 1;
  }

  std::ifstream file(program_path);
  if (!file) {
    LOG(ERROR) << "Failed to open file.";
    return 1;
  }
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string sample_program = buffer.str();
  WinZigC::Lexer lexer(sample_program);
  auto tokens = lexer.get_tokens();

  WinZigC::Frontend::Parser parser(std::move(tokens));
  auto program = parser.parse();

  WinZigC::Visitor::CodeGenVisitor codegen_visitor(optimize, debug);
  codegen_visitor.codegen(*program, program_path);
  codegen_visitor.print_llvm_ir(program_path);

  return 0;
}

} // namespace WinZigC
