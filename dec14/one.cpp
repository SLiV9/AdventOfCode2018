//usr/bin/g++ -g -O2 dec14/one.cpp -o dec14/.one && time ./dec14/.one; exit

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
	constexpr size_t after = 260321;
	constexpr size_t total = after + 10;

	size_t size = 2;
	size_t alice = 0;
	size_t bob = 1;
	std::array<uint8_t, total + 1> recipes = {3, 7};

	while (size < total)
	{
		//for (size_t i = 0; i < size; i++)
		//{
		//	if (i == alice && i == bob)
		//	{
		//		std::cout << "{" << int(recipes[i]) << "}";
		//	}
		//	else if (i == alice)
		//	{
		//		std::cout << "(" << int(recipes[i]) << ")";
		//	}
		//	else if (i == bob)
		//	{
		//		std::cout << "[" << int(recipes[i]) << "]";
		//	}
		//	else
		//	{
		//		std::cout << " " << int(recipes[i]) << " ";
		//	}
		//}
		//std::cout << std::endl;

		int flavor = recipes[alice] + recipes[bob];
		if (flavor >= 10)
		{
			recipes[size++] = flavor / 10;
			recipes[size++] = flavor % 10;
		}
		else
		{
			recipes[size++] = flavor;
		}
		alice = (alice + 1 + recipes[alice]) % size;
		bob   = (bob   + 1 + recipes[bob  ]) % size;
	}

	std::cout << "Scores after " << after << ": ";
	for (size_t i = after; i < total; i++)
	{
		std::cout << int(recipes[i]);
	}
	std::cout << std::endl;
}
