#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer/lexer.h"

int main(int argc, char** argv) {
  // Check if a file path was provided
  if (argc < 2) {
    std::cerr << "Please provide a file path." << std::endl;
    return 1;
  }

  // Read the file path from the command line
  std::string filePath = argv[1];

  // Open the file
  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "Failed to open file." << std::endl;
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
