//usr/bin/g++ -g -O2 dec12/one.cpp -o dec12/.one && time ./dec12/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


#define BITC 1
#define BITR 2
#define BITT 4
#define BITL 8
#define BITK 16

#define BUFFER 5

constexpr uint8_t code(bool k, bool l, bool c, bool r, bool t)
{
	return (BITK * k) | (BITL * l) | (BITC * c) | (BITR * r) | (BITT * t);
}

void printPots(const std::vector<char>& pots, size_t left, int min, int max)
{
	std::cout << std::string(left, ' ');
	if (max > min + 1)
	{
		std::cout << "S" << std::string(max - min - 1, ' ') << "E";
	}
	else if (max == min + 1) std::cout << "S" << "E";
	else if (max == min) std::cout << "X";
	else {}
	std::cout << std::endl;

	for (char pot : pots)
	{
		std::cout << (pot ? '#' : '.');
	}
	std::cout << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec12/input.txt");

	size_t zero = BUFFER;
	size_t left = BUFFER;
	size_t right = BUFFER;
	int min = 0;
	int max = -1;
	std::vector<char> pots(left, false);
	pots.reserve(left + BUFFER);

	std::string line;
	std::getline(file, line);
	{
		std::stringstream strm(line.substr(strlen("initial state: ")));
		char c;
		while (strm >> c)
		{
			pots.emplace_back(c == '#');
			max++;
		}
	}

	pots.resize(left + (max - min + 1) + right, false);

	// Empty line.
	std::getline(file, line);

	std::bitset<32> map;

	while (std::getline(file, line))
	{
		char k, l, c, r, t, n;
		sscanf(line.c_str(), "%c%c%c%c%c => %c", &k, &l, &c, &r, &t, &n);
		uint8_t x = code(k == '#', l == '#', c == '#', r == '#', t == '#');
		map[x] = (n == '#');
	}

	std::vector<char> newpots(pots.size(), false);

	size_t generations = 20;
	for (size_t g = 0; g < generations; g++)
	{
		printPots(pots, left, min, max);

		for (int i = min - 1; i <= max + 1; i++)
		{
			uint8_t x = code(
				pots[zero + i - 2],
				pots[zero + i - 1],
				pots[zero + i],
				pots[zero + i + 1],
				pots[zero + i + 2]);
			newpots[zero + i] = map[x];
		}

		pots.swap(newpots);
		std::fill(newpots.begin(), newpots.end(), false);

		if (pots[zero + max + 1])
		{
			right--;

			if (right < 3)
			{
				right = BUFFER;
				pots.resize(left + (max - min + 1) + right, false);
				newpots.resize(left + (max - min + 1) + right, false);
			}

			max++;
		}

		if (pots[zero + min - 1])
		{
			left--;

			if (left < 3)
			{
				size_t curlen = left + (max - min + 1) + right;
				pots.resize(curlen + BUFFER, false);
				std::rotate(pots.begin(), pots.begin() + curlen, pots.end());
				newpots.resize(curlen + BUFFER, false);
				zero += BUFFER;
				left += BUFFER;
			}

			min--;
		}
	}

	printPots(pots, left, min, max);

	int sum = 0;
	for (int i = min; i <= max; i++)
	{
		if (pots[zero + i]) sum += i;
	}
	std::cout << "Sum: " << sum << std::endl;
}
