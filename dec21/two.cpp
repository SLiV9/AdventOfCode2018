//usr/bin/g++ -g -O2 dec21/two.cpp -o dec21/.two && time ./dec21/.two; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


int main(int /*argc*/, char* /*argv*/[])
{
	std::array<int, 6> reg;
	reg.fill(0);

	int first = 0;
	int last = 0;
	int halters = 0;
	int halterupdate = 1;

	std::bitset<16777216> visited;

	reg[1] = 0;
	while (true)
	{
		reg[5] = reg[1] | 65536;
		reg[1] = 8586263;
		do
		{
			reg[1] += reg[5] & 255;
			reg[1] = reg[1] & 16777215;
			reg[1] *= 65899;
			reg[1] = reg[1] & 16777215;
			reg[5] = reg[5] >> 8;
		}
		while (reg[5] > 0);

/* INSERTION */
if (visited[reg[1]])
{
	std::cout << last << " halts as halter number " << halters << std::endl;
	halters++;
	std::cout << reg[1] << " halts as halter number " << halters << std::endl;
	return 0;
}

halters++;
visited[reg[1]] = true;

if (halters == halterupdate)
{
	std::cout << halters << std::endl;
	halterupdate *= 2;
}

if (halters > 16777215)
{
	std::cout << "Kak" << std::endl;
	return -1;
}

if (first == 0)
{
	first = reg[1];
	std::cout << first << " halts first" << std::endl;
}
else
{
	last = reg[1];
}
/* INSERTION */

	}
}

// The only reason why there would be a 'last' input for which the program
// halts, is if there is a cycle and the one after the last is the first again.
