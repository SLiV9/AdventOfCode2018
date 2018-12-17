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


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec17/test.txt");

	constexpr size_t width = 1000;
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

	for (const auto& row : board)
	{
		std::cout.write(row.data() + minx, (maxx - minx + 1));
		std::cout << std::endl;
	}
	std::cout << std::endl;

	int reached = 0;

	// TODO

	std::cout << "The water reaches " << reached << " tiles" << std::endl;
}
