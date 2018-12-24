//usr/bin/g++ -g -O2 dec22/one.cpp -o dec22/.one && time ./dec22/.one; exit

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
	int depth;
	int targetx;
	int targety;
	{
		std::ifstream file("dec22/input.txt");
		std::string line;
		std::getline(file, line);
		sscanf(line.c_str(), "depth: %d", &depth);
		std::getline(file, line);
		sscanf(line.c_str(), "target: %d,%d", &targetx, &targety);
	}

	static int N = 20183;

	std::vector<std::vector<int>> geo(targety + 1);
	std::vector<std::vector<int>> erosion(targety + 1);
	for (int y = 0; y <= targety; y++)
	{
		geo[y].resize(targetx + 1);
		erosion[y].resize(targetx + 1);
		for (int x = 0; x <= targetx; x++)
		{
			if (y == 0)
			{
				if (x == 0)
				{
					geo[y][x] = 0;
				}
				else
				{
					geo[y][x] = (x * 16807) % N;
				}
			}
			else
			{
				if (x == 0)
				{
					geo[y][x] = (y * 48271) % N;
				}
				else
				{
					geo[y][x] = (erosion[y-1][x] * erosion[y][x-1]) % N;
				}
			}
			erosion[y][x] = (geo[y][x] + depth) % N;
		}
	}
	geo[targety][targetx] = depth;
	erosion[targety][targetx] = (geo[targety][targetx] + depth) % N;

	for (int y = 0; y <= targety; y++)
	{
		for (int x = 0; x <= targetx; x++)
		{
			switch (erosion[y][x] % 3)
			{
				case 0: std::cout << "."; break;
				case 1: std::cout << "="; break;
				case 2: std::cout << "|"; break;
			}
		}
		std::cout << std::endl;
	}

	int sum = 0;
	for (int y = 0; y <= targety; y++)
	{
		for (int x = 0; x <= targetx; x++)
		{
			sum += (erosion[y][x] % 3);
		}
	}

	std::cout << "Sum: " << sum << std::endl;
}

// 6170 is too low
// 6209 is too high
