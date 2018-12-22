//usr/bin/g++ -g -O2 dec21/one.cpp -o dec21/.one && time ./dec21/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


#include "input.h"

int main(int /*argc*/, char* /*argv*/[])
{
	std::array<int, regsize_input> reg;

	for (int i = 1; i < 10; i++)
	{
		reg.fill(0);
		reg[0] = i;
		int ticks = run_input(reg);
		std::cout << i << " halts after " << ticks << " ticks" << std::endl;
	}

	std::cout << "The answer is " << reg[0] << std::endl;
}
