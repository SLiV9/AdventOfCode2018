//usr/bin/g++ -g -O2 dec23/one.cpp -o dec23/.one && time ./dec23/.one; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


struct Bot
{
	int x;
	int y;
	int z;
	int r;

	int distanceTo(const Bot& other) const
	{
		return (std::abs(x - other.x) + std::abs(y - other.y)
			+ std::abs(z - other.z));
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec23/input.txt");

	std::vector<Bot> bots;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int x, y, z, r;
		sscanf(line.c_str(), "pos=<%d,%d,%d>, r=%d",
			&x, &y, &z, &r);
		bots.push_back({x, y, z, r});
	}

	int bestrange = -1;
	size_t bestindex = 0;

	for (size_t i = 0; i < bots.size(); i++)
	{
		if (bots[i].r > bestrange)
		{
			bestrange = bots[i].r;
			bestindex = i;
		}
	}

	std::cout << "Best bot is bots[" << bestindex << "]"
		" with range " << bestrange << std::endl;

	int inrange = 0;
	for (const Bot& bot : bots)
	{
		if (bots[bestindex].distanceTo(bot) <= bestrange)
		{
			inrange++;
		}
	}

	std::cout << "In range: " << inrange << std::endl;
}
