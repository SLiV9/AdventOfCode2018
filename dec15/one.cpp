//usr/bin/g++ -g -O2 dec15/one.cpp -o dec15/.one && time ./dec15/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


struct Unit
{
	char type;
	uint8_t xenon;
	uint8_t yahoo;
	uint8_t hitpoints;

	Unit(char t, uint8_t x, uint8_t y) :
		type(t),
		xenon(x),
		yahoo(y),
		hitpoints(200)
	{}
};

constexpr bool operator<(const Unit& a, const Unit& b)
{
	return (a.yahoo < b.yahoo) || (a.yahoo == b.yahoo && a.xenon < b.xenon);
}

constexpr bool operator<(const Unit& a, uint8_t y)
{
	return (a.yahoo < y);
}

void print(std::vector<std::vector<char>>& board,
	std::vector<Unit>& elves, std::vector<Unit>& goblins)
{
	// Pre-condition: elves and goblins are sorted.
	auto elf = elves.begin();
	auto goblin = goblins.begin();

	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			std::cout << board[y][x];
		}
		const char* sep = "   ";
		for (size_t x = 0; x < board[y].size(); x++)
		{
			char c = board[y][x];
			if (c == 'E' && elf != elves.end()
				&& elf->yahoo == y && elf->xenon == x)
			{
				std::cout << sep << "E(" << int(elf->hitpoints) << ")";
				sep = ", ";
				++elf;
			}
			else if (c == 'G' && goblin != elves.end()
				&& goblin->yahoo == y && goblin->xenon == x)
			{
				std::cout << sep << "G(" << int(goblin->hitpoints) << ")";
				sep = ", ";
				++goblin;
			}
		}
		std::cout << std::endl;
	}
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec15/test.txt");

	std::vector<std::vector<char>> board;
	std::vector<std::vector<uint8_t>> hmap;
	std::vector<Unit> elves;
	std::vector<Unit> goblins;

	std::string line;
	while (std::getline(file, line))
	{
		board.emplace_back();

		std::stringstream strm(line);
		for (char x : line)
		{
			if (x == 'E')
			{
				elves.emplace_back(x, board.back().size(), board.size() - 1);
			}
			else if (x == 'G')
			{
				goblins.emplace_back(x, board.back().size(), board.size() - 1);
			}

			board.back().emplace_back(x);
		}

		hmap.emplace_back(board.back().size());
	}

	size_t rounds = 0;
	while (true)
	{
		std::sort(elves.begin(), elves.end());
		std::sort(goblins.begin(), goblins.end());
		std::cout << "After " << rounds << " rounds:" << std::endl;
		print(board, elves, goblins);

		auto elf = elves.begin();
		auto goblin = goblins.begin();

		break;

		rounds++;
	}

	std::cout << "Completed " << rounds << " full rounds" << std::endl;
}

// I was thinking about an optimization where I just do a floodfill and take
// any step that brings me closest to any unit, but this does not always work:
// #######
// #...0G#
// #..#1.#
// #12E2.#
// #0....#
// #G....#
// #######
// Here the optimization would lead me to go left, because it is earlier in
// reading order than going right, but we should go right because it leads
// to the upper goblin, which is earlier in reading order than the lower goblin.
