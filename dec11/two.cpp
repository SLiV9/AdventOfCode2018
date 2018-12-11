//usr/bin/g++ -g -O2 dec11/two.cpp -o dec11/.two && time ./dec11/.two; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


constexpr int serial = 7803;

constexpr int value(int i, int j)
{
	return (((((i + 10) * j + serial) * (i + 10)) / 100) % 10) - 5;
}

int main(int /*argc*/, char* /*argv*/[])
{
	int bestscore = 0;
	int besti;
	int bestj;
	int bestw;

	constexpr int CHUNKW = 5;
	std::array<std::array<int, 300 / CHUNKW>, 300 / CHUNKW> chunk;

	// We know w is at least 2 because the top 3x3 score was 31, and the maximum
	// value of a cell is 4, so you need at least 8 cells, hence at least 3x3.
	for (int w = 2; w < 300; w++)
	{
		for (int i = 0; i + w < 300; i++)
		{
			for (int j = 0; j + w < 300; j++)
			{
				int score = 0;

				int i0 = i + CHUNKW - 1 - ((i + CHUNKW - 1) % CHUNKW);
				int i1 = i + w - ((i + w) % CHUNKW);
				int j0 = j - ((j + CHUNKW - 1) % CHUNKW) - 1;
				int j1 = j + w - ((j + w) % CHUNKW);

				for (int ii = i; ii <= i + w && ii < i0; ii++)
				{
					for (int jj = j; jj <= j + w; jj++)
					{
						score += value(ii + 1, jj + 1);
					}
				}

				for (int ii = i0; ii < i1; ii++)
				{
					for (int jj = j; jj <= j + w && jj < j0; jj++)
					{
						score += value(ii + 1, jj + 1);
					}
				}

				for (int ci = i0 / CHUNKW; ci < i1 / CHUNKW; ci++)
				{
					for (int cj = j0 / CHUNKW; cj < j1 / CHUNKW; cj++)
					{
						score += chunk[ci][cj];
					}
				}

				for (int ii = i1; ii <= i + w; ii++)
				{
					for (int jj = j; jj <= j + w; jj++)
					{
						score += value(ii + 1, jj + 1);
					}
				}

				for (int ii = i0; ii < i1; ii++)
				{
					for (int jj = j1; jj <= j + w; jj++)
					{
						score += value(ii + 1, jj + 1);
					}
				}

				if (w + 1 == CHUNKW && i % CHUNKW == 0 && j % CHUNKW == 0)
				{
					chunk[i / CHUNKW][j / CHUNKW] = score;
				}

				if (score > bestscore)
				{
					bestscore = score;
					besti = i + 1;
					bestj = j + 1;
					bestw = w + 1;
				}
			}
		}

		std::cout << w << " // " << besti << "," << bestj << "," << bestw
			<< ": " << bestscore << std::endl;
	}

	for (int j = bestj - 1; j < bestj + bestw + 1; j++)
	{
		for (int i = besti - 1; i < besti + bestw + 1; i++)
		{
			if (bestj < 1 || bestj > 300 || besti < 1 || besti > 300)
			{
				std::cout << " " << " #";
			}
			else std::cout << " " << std::setw(2) << value(i, j);
		}
		std::cout << std::endl;
	}

	std::cout << besti << "," << bestj << "," << bestw
		<< ": " << bestscore << std::endl;
}

// a b b d
// a c c d
// a c c d
// a e e d

//  3-12 =  3- 4  5- 9 10-12
//  5-12 =        5- 9 10-12
//  5- 9 =        5- 9
//  6- 8 =  6- 8
//  6-12 =  6- 9       10-12
