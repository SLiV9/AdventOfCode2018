//usr/bin/g++ -g -O2 dec21/input.cpp -o dec21/.input && time dec21/.input; exit

#include <array>

#define REGSIZE 6

void run(std::array<int, REGSIZE>& reg)
{
}

#ifndef MAIN
#define MAIN
int main(int /*argc*/, char* /*argv*/[])
{
  std::array<int REGSIZE> reg;
  reg.fill(0);
  run(reg);
  std::cout << reg[0] << std::endl;
}
#endif
