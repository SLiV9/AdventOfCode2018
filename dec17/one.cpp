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

void print(std::vector<std::array<char, width>>& board, int minx, int maxx,
	int miny, int maxy)
{
	for (int y = miny; y <= maxy; y++)
	{
		std::cout.write(board[y].data() + minx, (maxx - minx + 1));
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec17/input.txt");

	std::vector<std::array<char, width>> board;
	board.emplace_back();
	board[0].fill('.');
	int maxdepth = 0;
	int miny = 999;
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

				if (y1 < miny) miny = y1;

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

				if (y0 < miny) miny = y0;

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

	std::cout << "Spring at row " << (miny - 1) << std::endl;
	board[miny - 1][500] = '+';

	int reached = 0;
	std::vector<std::pair<int, int>> sources = {{500, miny - 1}};
	while (!sources.empty())
	{
		bool anychanges = false;

		int srcx = sources.back().first;
		int srcy = sources.back().second;

		if (board[srcy][srcx] != '+')
		{
			sources.pop_back();
			continue;
		}

		//print(board, std::max(minx, srcx - 100), std::min(maxx, srcx + 100),
		//	std::max(0, srcy - 50), std::min(maxdepth, srcy + 50));

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
					anychanges = true;
				}
				break;

				case '|':
				case '+':
				case '*':
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
			board[srcy][srcx] = '*';
			sources.pop_back();
			continue;
		}

		y--;
		int leftwall = -1;
		int rightwall = -1;
		int leftsrc = -1;
		int rightsrc = -1;
		int leftinf = -1;
		int rightinf = -1;

		for (int x = srcx - 1; x >= minx; x--)
		{
			switch (board[y][x])
			{
				case '.':
				{
					board[y][x] = '|';
					reached += 1;
					anychanges = true;
				}
				break;

				case '|':
				case '+':
				break;

				case '*':
				{
					leftinf = x;
				}
				break;

				case '#':
				case '~':
				{
					leftwall = x;
				}
				break;
			}
			if (leftinf != -1) break;
			if (leftwall != -1) break;

			switch (board[y + 1][x])
			{
				case '.':
				case '|':
				case '+':
				case '*':
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
					anychanges = true;
				}
				break;

				case '|':
				case '+':
				break;

				case '*':
				{
					rightinf = x;
				}
				break;

				case '#':
				case '~':
				{
					rightwall = x;
				}
				break;
			}
			if (rightinf != -1) break;
			if (rightwall != -1) break;

			switch (board[y + 1][x])
			{
				case '.':
				case '|':
				case '+':
				case '*':
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
				anychanges = (board[y][x] != '~');
				board[y][x] = '~';
			}
		}

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

		if (!anychanges)
		{
			board[srcy][srcx] = '*';
			sources.pop_back();
		}
	}

	print(board, std::max(minx, 400), std::min(maxx, 600));

	std::cout << "The water reaches " << reached << " tiles" << std::endl;
}
