//usr/bin/g++ -g -O2 dec17/one.cpp -o dec17/.one && time ./dec17/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


constexpr size_t width = 1000;

void print(std::vector<std::array<char, width>>& board, int minx, int maxx)
{
	for (const auto& row : board)
	{
		std::cout.write(row.data() + minx, (maxx - minx + 1));
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec17/test.txt");

	std::vector<std::array<char, width>> board;
	board.emplace_back();
	board[0].fill('.');
	board[0][500] = '+';
	int maxdepth = 0;
	int minx = 500;
	int maxx = 500;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		switch (line[0])
		{
			case 'x':
			{
				int x0, y1, y2;
				sscanf(line.c_str(), "x=%d, y=%d..%d", &x0, &y1, &y2);

				if (x0 - 1 < minx) minx = x0 - 1;
				if (x0 + 1 > maxx) maxx = x0 + 1;

				if (y2 > maxdepth)
				{
					board.resize(y2 + 1);
					for (int y = maxdepth + 1; y <= y2; y++)
					{
						board[y].fill('.');
					}
					maxdepth = y2;
				}

				for (int y = y1; y <= y2; y++)
				{
					board[y][x0] = '#';
				}
			}
			break;
			case 'y':
			{
				int y0, x1, x2;
				sscanf(line.c_str(), "y=%d, x=%d..%d", &y0, &x1, &x2);

				if (x1 - 1 < minx) minx = x1 - 1;
				if (x1 + 1 > maxx) maxx = x1 + 1;
				if (x2 - 1 < minx) minx = x2 - 1;
				if (x2 + 1 > maxx) maxx = x2 + 1;

				if (y0 > maxdepth)
				{
					board.resize(y0 + 1);
					for (int y = maxdepth + 1; y <= y0; y++)
					{
						board[y].fill('.');
					}
					maxdepth = y0;
				}

				for (int x = x1; x <= x2; x++)
				{
					board[y0][x] = '#';
				}
			}
			break;
			default:
			{
				std::cout << "'" << line << "'" << std::endl;
				assert(false);
			}
			break;
		}
	}

	int reached = 0;
	std::vector<std::pair<int, int>> sources = {{500, 0}};
	while (!sources.empty())
	{
		print(board, minx, maxx);

		int srcx = sources.back().first;
		int srcy = sources.back().second;
		int y = srcy + 1;
		for (; y <= maxdepth; y++)
		{
			bool running = true;
			switch (board[y][srcx])
			{
				case '.':
				{
					board[y][srcx] = '|';
					reached += 1;
				}
				break;

				case '|':
				break;

				case '#':
				case '~':
				{
					running = false;
				}
				break;
			}
			if (!running) break;
		}

		if (y > maxdepth)
		{
			sources.pop_back();
			continue;
		}

		y--;
		int leftwall = -1;
		int rightwall = -1;
		int leftsrc = -1;
		int rightsrc = -1;

		for (int x = srcx - 1; x >= minx; x--)
		{
			switch (board[y][x])
			{
				case '.':
				{
					board[y][x] = '|';
					reached += 1;
				}
				break;

				case '|':
				break;

				case '#':
				case '~':
				{
					leftwall = x;
				}
				break;
			}
			if (leftwall != -1) break;

			switch (board[y + 1][x])
			{
				case '.':
				case '|':
				{
					leftsrc = x;
				}
				break;

				case '#':
				case '~':
				break;
			}
			if (leftsrc != -1) break;
		}

		for (int x = srcx + 1; x <= maxx; x++)
		{
			switch (board[y][x])
			{
				case '.':
				{
					board[y][x] = '|';
					reached += 1;
				}
				break;

				case '|':
				break;

				case '#':
				case '~':
				{
					rightwall = x;
				}
				break;
			}
			if (rightwall != -1) break;

			switch (board[y + 1][x])
			{
				case '.':
				case '|':
				{
					rightsrc = x;
				}
				break;

				case '#':
				case '~':
				break;
			}
			if (rightsrc != -1) break;
		}

		if (leftwall != -1 && rightwall != -1)
		{
			for (int x = leftwall + 1; x <= rightwall - 1; x++)
			{
				board[y][x] = '~';
			}
		}
		else
		{
			sources.pop_back();

			if (leftsrc != -1)
			{
				board[y][leftsrc] = '+';
				sources.emplace_back(leftsrc, y);
			}

			if (rightsrc != -1)
			{
				board[y][rightsrc] = '+';
				sources.emplace_back(rightsrc, y);
			}
		}
	}

	print(board, minx, maxx);

	std::cout << "The water reaches " << reached << " tiles" << std::endl;
}
