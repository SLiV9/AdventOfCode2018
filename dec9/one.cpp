//usr/bin/g++ -g -O2 dec9/one.cpp -o dec9/.one && time ./dec9/.one; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec9/input.txt");

	std::string line;
	std::getline(file, line);

	int players, maxvalue;
	sscanf(line.c_str(), "%d players; last marble is worth %d points",
		&players, &maxvalue);

	std::vector<int> scores(players, 0);

	std::vector<int> marbles = { 0 };
	marbles.reserve(maxvalue + 1);
	size_t curpos = 0;

	for (int value = 1; value <= maxvalue; value++)
	{
		if (value % 23 == 0)
		{
			curpos = (curpos + marbles.size() - 7) % marbles.size();

			int player = value % players;
			scores[player] += value;
			scores[player] += marbles[curpos];

			marbles.erase(marbles.begin() + curpos);
			if (curpos == marbles.size()) curpos = 0;
		}
		else
		{
			curpos = ((curpos + 1) % marbles.size()) + 1;
			marbles.emplace(marbles.begin() + curpos, value);
		}
	}

	int maxscore = 0;
	for (int score : scores)
	{
		if (score > maxscore)
		{
			maxscore = score;
		}
	}

	std::cout << maxscore << std::endl;
}
