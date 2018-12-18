//usr/bin/g++ -g -O2 dec18/two.cpp -o dec18/.two && time ./dec18/.two; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


struct Board
{
	std::bitset<2500> trees;
	std::bitset<2500> yards;

	bool operator==(const Board& other)
	{
		return (trees == other.trees && yards == other.yards);
	}
};

void print(const Board& board, size_t width, size_t height)
{
	std::cout << std::string(width, '=') << std::endl;
	for (size_t r = 0; r < height; r++)
	{
		for (size_t c = 0; c < width; c++)
		{
			if      (board.trees[r * width + c]) std::cout << '|';
			else if (board.yards[r * width + c]) std::cout << '#';
			else                                 std::cout << ".";
		}
		std::cout << std::endl;
	}
	std::cout << std::string(width, '=') << std::endl;
}

int score(const Board& board)
{
	return board.trees.count() * board.yards.count();
}

uint32_t hash(const Board& board, size_t width, size_t height)
{
	uint32_t hh = 0; // 8 bits

	for (size_t r = 0; r < height; r++)
	{
		for (size_t c = 0; c < width; c++)
		{
			hh = (37 * hh + board.yards[r * width + c]) % 251;
		}
	}

	uint32_t trees = board.trees.count(); // 12 bits
	uint32_t yards = board.yards.count(); // 12 bits

	// Note that 2^12 > 2500 = 50 * 50 and 8 + 12 + 12 = 32.
	return (hh << 24) | (trees << 12) | yards;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec18/input.txt");

	size_t width = 0;
	size_t height = 0;
	std::vector<Board> boards;
	std::vector<uint32_t> hashes;

	boards.emplace_back();

	size_t boardfill = 0;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		width = line.size();
		height++;

		for (size_t i = 0; i < line.size(); i++)
		{
			char x = line[i];
			if      (x == '|') boards.back().trees[boardfill + i] = true;
			else if (x == '#') boards.back().yards[boardfill + i] = true;
		}

		boardfill += width;
	}

	hashes.emplace_back(hash(boards.back(), width, height));

	constexpr size_t total = 1000000000;
	for (int t = 0; t < total; t++)
	{
		Board& board = boards.back();
		Board nextb;

		//std::cout << std::setw(8) << std::setfill('0') << std::hex
		//	<< hash(board, width, height) << std::dec << std::endl;
		//print(board, width, height);

		for (size_t r = 0; r < height; r++)
		{
			for (size_t c = 0; c < width; c++)
			{
				int trees = 0;
				int yards = 0;

				for (size_t rr = ((r > 0) ? (r - 1) : r);
					rr <= ((r + 1 < height) ? (r + 1) : r);
					rr++)
				{
					for (size_t cc = ((c > 0) ? (c - 1) : c);
						cc <= ((c + 1 < width) ? (c + 1) : c);
						cc++)
					{
						if (rr == r && cc == c) continue;

						if      (board.trees[rr * width + cc]) trees++;
						else if (board.yards[rr * width + cc]) yards++;
					}
				}

				if      (board.trees[r * width + c])
				{
					if (yards >= 3)
					{
						nextb.yards[r * width + c] = true;
					}
					else
					{
						nextb.trees[r * width + c] = true;
					}
				}
				else if (board.yards[r * width + c])
				{
					if (trees && yards)
					{
						nextb.yards[r * width + c] = true;
					}
				}
				else
				{
					if (trees >= 3)
					{
						nextb.trees[r * width + c] = true;
					}
				}
			}
		}

		uint32_t nexthash = hash(nextb, width, height);

		size_t repeat = size_t(-1);
		bool collision = false;
		for (size_t i = 0; i < boards.size(); i++)
		{
			if (nexthash != hashes[i]) continue;

			if (nextb == boards[i])
			{
				std::cout << "Repeat between " << i << " ("
					<< std::setw(8) << std::setfill('0') << std::hex
					<< hashes[i] << std::dec << ")" << std::endl;
				print(boards[i], width, height);

				repeat = i;
				break;
			}
			else
			{
				std::cout << "Collision between " << i << " ("
					<< std::setw(8) << std::setfill('0') << std::hex
					<< hashes[i] << std::dec << ")" << std::endl;
				print(boards[i], width, height);

				collision = true;
			}
		}

		if (repeat != size_t(-1) || collision)
		{
			std::cout << " and " << boards.size() << " ("
					<< std::setw(8) << std::setfill('0') << std::hex
					<< nexthash << std::dec << ")" << std::endl;
			print(nextb, width, height);
		}

		if (repeat != size_t(-1))
		{
			// nextb = boards[boards.size()]
			// t = board.size() - 1
			// nextb = boards[t + 1]
			// 0 <= repeat < t + 1
			// 0 < (t + 1 - repeat) <= t + 1
			size_t period = (t + 1 - repeat);
			// we seek boards[total]
			// total = t + 1 + (total - (t + 1))
			size_t remaining = total - (t + 1);
			// boards[total] = boards[t + 1 + remaining]
			// boards[t + 1] = boards[repeat]
			// boards[total] = boards[repeat + (remaining % period)]
			size_t target = repeat + (remaining % period);

			std::cout << "Copying " << target << std::endl;
			hashes.emplace_back(hashes[target]);
			nextb = boards[target];
			boards.emplace_back(std::move(nextb));
			break;
		}

		hashes.emplace_back(nexthash);
		boards.emplace_back(std::move(nextb));
	}

	std::cout << std::setw(8) << std::setfill('0') << std::hex << hashes.back()
		<< std::dec << std::endl;
	print(boards.back(), width, height);

	std::cout << "Score: " << score(boards.back()) << std::endl;
}
