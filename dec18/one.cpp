//usr/bin/g++ -g -O2 dec18/one.cpp -o dec18/.one && time ./dec18/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


void print(std::vector<std::string>& board)
{
	for (const auto& row : board)
	{
		std::cout << row << std::endl;
	}
	std::cout << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec18/input.txt");

	std::vector<std::string> board;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		board.emplace_back(line);
	}

	int height = board.size();
	int width = board[0].size();

	std::vector<std::string> nextb = board;

	for (int t = 0; t < 10; t++)
	{
		print(board);

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

						if      (board[rr][cc] == '|') trees++;
						else if (board[rr][cc] == '#') yards++;
					}
				}

				switch (board[r][c])
				{
					case '.':
					{
						nextb[r][c] = (trees >= 3) ? '|' : '.';
					}
					break;
					case '|':
					{
						nextb[r][c] = (yards >= 3) ? '#' : '|';
					}
					break;
					case '#':
					{
						nextb[r][c] = (trees && yards) ? '#' : '.';
					}
					break;
				}
			}
		}

		board.swap(nextb);
	}

	print(board);

	int trees = 0;
	int yards = 0;

	for (const auto& row : board)
	{
		for (char x : row)
		{
			if      (x == '|') trees++;
			else if (x == '#') yards++;
		}
	}

	std::cout << trees << " x " << yards << " = "
		<< (trees * yards) << std::endl;
}
