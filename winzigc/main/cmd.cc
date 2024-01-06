#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "winzigc/frontend/lexer/lexer.h"

#include "glog/logging.h"

int main(int argc, char** argv) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // Check if a file path was provided
  if (argc < 2) {
    LOG(ERROR) << "Please provide a file path.";
    return 1;
  }

  // Read the file path from the command line
  std::string filePath = argv[1];

  // Open the file
  std::ifstream file(filePath);
  if (!file) {
    LOG(ERROR) << "Failed to open file.";
    return 1;
  }

  // Read the file content into a string
  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string sample_program = buffer.str();
  WinZigC::Lexer lexer(sample_program);
  auto tokens = lexer.get_tokens();

  for (auto token : tokens) {
    std::cout << token.lexeme << std::endl;
  }

  return 0;
}
