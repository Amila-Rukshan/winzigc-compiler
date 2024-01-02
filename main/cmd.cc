#include <ctime>
#include <iostream>

#include "lexer/lexer.h"

int main(int argc, char** argv) {
  std::string sample_program = R"(
{ This program tests simple function calls for many argument. }
program MyProg:

function Calc(x,y,z:integer):integer;
	begin
		return x + y * z;
	end 
Calc;

begin
	output(Calc(3, 4, -5));
end 

MyProg.
)";
  WinZigC::Lexer lexer(sample_program);

  std::cout << "WinzigC" << std::endl;
  return 0;
}
