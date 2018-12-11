//usr/bin/g++ -g -O2 dec11/one.cpp -o dec11/.one && time ./dec11/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


static constexpr int serial = 7803;

constexpr int value(int i, int j)
{
	return (((((i + 10) * j + serial) * (i + 10)) / 100) % 10) - 5;
}

int main(int /*argc*/, char* /*argv*/[])
{
	int bestscore = 0;
	int besti;
	int bestj;

	for (int i = 1; i + 2 <= 300; i++)
	{
		for (int j = 1; j + 2 <= 300; j++)
		{
			int score = 0;

			for (int ii = i; ii <= i + 2; ii++)
			{
				for (int jj = j; jj <= j + 2; jj++)
				{
					score += value(ii, jj);
				}
			}

			if (score > bestscore)
			{
				bestscore = score;
				besti = i;
				bestj = j;
			}
		}
	}

	for (int j = bestj - 1; j <= bestj + 2 + 1; j++)
	{
		for (int i = besti - 1; i <= besti + 2 + 1; i++)
		{
			if (bestj < 1 || bestj > 300 || besti < 1 || besti > 300)
			{
				std::cout << " " << " #";
			}
			else std::cout << " " << std::setw(2) << value(i, j);
		}
		std::cout << std::endl;
	}

	std::cout << besti << "," << bestj << ": " << bestscore << std::endl;
}
