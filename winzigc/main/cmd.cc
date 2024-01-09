#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "winzigc/frontend/lexer/lexer.h"
#include "winzigc/frontend/parser/parser.h"

#include "glog/logging.h"

int main(int argc, char** argv) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  if (argc < 2) {
    LOG(ERROR) << "Please provide a file path.";
    return 1;
  }

  std::string filePath = argv[1];
  std::ifstream file(filePath);
  if (!file) {
    LOG(ERROR) << "Failed to open file.";
    return 1;
  }
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string sample_program = buffer.str();
  WinZigC::Lexer lexer(sample_program);
  auto tokens = lexer.get_tokens();

  for (auto token : *tokens) {
    std::cout << token.lexeme << std::endl;
  }

  WinZigC::Frontend::Parser parser(std::move(tokens));
  auto program = parser.parse();

  return 0;
}
