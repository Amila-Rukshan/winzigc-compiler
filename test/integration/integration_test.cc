#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <map>
#include <thread>
#include <chrono>

#include "winzigc/main/cmd.h"

#include "gtest/gtest.h"

namespace WinZigC {

struct ProgramTest {
  std::string input;
  std::string output;
};

static const std::vector<ProgramTest> program_test = {
    {"12", "1\n2\n3\n4\n6\n12\n"},
    {"541", "1\n"},
    {"100\n127\n607\n", "0\n1\n2\n"},
    {"", "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n"},
    {"3\n", "27 1 \n"},
    {"7\n", "5040 8 \n"},
    {"", "1\n1\n2\n3\n5\n8\n13\n"},
    {"3\n", "1 3 \n1 2 \n3 2 \n1 3 \n2 1 \n2 3 \n1 3 \n"},
    {"1\n2\n3\n2\n", "1 2 4 \n3 2 29 \n"},
    {"", "1\n0\n"},
    {"13\n14\n", "0\n1\n"},
    {"100\n20\n-34\n10\n0\n29\n", "-34\n0\n10\n20\n29\n100\n"},
    {"234\n100\n", "2\n0\n"},
    {"77\n100\n", "1\n0\n"},
    {"100/5-7*3\n", "-1\n"},
    {"127\n100\n", "1\n0\n"},
    {"127\n100\n", "true\nfalse\n"},
    {"127\n100\n", "1\n0\n"},
    {"", "0\n1\n0\n"},
    {"", "0\n13\n0\n"},
    {"", "0\n13\n0\n"},
    {"", "13 -2 \n-2 -2 \n0\n13 -1 \n"},
    {"", "13 -2 10 43 \n-2 10 -2 43 \n1\n13 -2 10 11 \n"},
    {"71\n100\n546\n", "1\n0\n2\n"},
    {"", "13 -2 \n-2 -2 \n0\n13 -1 \n"},
    {"128\n96\n", "32\n"},
};

std::string exec_binary(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

TEST(IntegrationTest, GenerateBitcodeAndBinary) {
  for (int i = 1; i <= 26; ++i) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << i;
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path program_path = current_path / ("example-programs/winzig_" + oss.str());

    std::vector<std::string> args = {"winzigc-compiler", "-opt", program_path.string()};
    std::vector<char*> argv;
    for (const auto& arg : args) {
      argv.push_back(const_cast<char*>(arg.data()));
    }
    argv.push_back(nullptr);

    int winzigc_result = main(argv.size() - 1, argv.data());
    EXPECT_EQ(winzigc_result, 0);

    std::string winzigc_binary_path = program_path.string() + "_binary";
    std::string command = "clang -O3 " + program_path.string() + ".ll -o " + winzigc_binary_path;
    int clang_result = std::system(command.c_str());
    EXPECT_EQ(clang_result, 0);

    std::string exec_command =
        "echo \"" + program_test[i - 1].input + "\" | " + winzigc_binary_path;
    std::string kill_command = "sleep 1 && pkill -f " + winzigc_binary_path;
    std::thread command_thread([kill_command] { std::system(kill_command.c_str()); });
    std::string output = exec_binary(exec_command.c_str());
    command_thread.join();
    EXPECT_TRUE(output.compare(0, program_test[i - 1].output.size(), program_test[i - 1].output) ==
                0);

    std::filesystem::remove(program_path.string() + ".ll");
    std::filesystem::remove(winzigc_binary_path);
  }
}

} // namespace WinZigC
