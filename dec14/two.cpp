//usr/bin/g++ -g -O2 dec14/two.cpp -o dec14/.two && time ./dec14/.two; exit

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
	constexpr const char* target = "260321";

	size_t size = 2;
	size_t alice = 0;
	size_t bob = 1;
	size_t buffersize = 65536;
	std::vector<uint8_t> recipes = {3, 7};
	recipes.resize(buffersize);

	size_t solution = size_t(-1);
	while (solution == size_t(-1))
	{
		// for (size_t i = 0; i < size; i++)
		// {
		// 	if (i == alice && i == bob)
		// 	{
		// 		std::cout << "{" << int(recipes[i]) << "}";
		// 	}
		// 	else if (i == alice)
		// 	{
		// 		std::cout << "(" << int(recipes[i]) << ")";
		// 	}
		// 	else if (i == bob)
		// 	{
		// 		std::cout << "[" << int(recipes[i]) << "]";
		// 	}
		// 	else
		// 	{
		// 		std::cout << " " << int(recipes[i]) << " ";
		// 	}
		// }
		// std::cout << std::endl;

		if (size + 2 > buffersize)
		{
			buffersize *= 2;
			recipes.resize(buffersize);
			std::cout << "Resizing to " << buffersize << std::endl;
		}

		bool plustwo = false;
		int flavor = recipes[alice] + recipes[bob];
		if (flavor >= 10)
		{
			recipes[size++] = flavor / 10;
			recipes[size++] = flavor % 10;
			plustwo = true;
		}
		else
		{
			recipes[size++] = flavor;
		}
		alice = (alice + 1 + recipes[alice]) % size;
		bob   = (bob   + 1 + recipes[bob  ]) % size;

		if (size < strlen(target)) continue;

		for (size_t s = size - strlen(target) - plustwo;
			s <= size - strlen(target); s++)
		{
			bool matches = true;
			for (size_t i = 0; i < strlen(target); i++)
			{
				if (recipes[s + i] != (target[i] - '0'))
				{
					matches = false;
					break;
				}
			}

			if (matches)
			{
				solution = s;
				break;
			}
		}
	}

	std::cout << "There are " << solution << " recipes before " << target
		<< " is first reached." << std::endl;
}
