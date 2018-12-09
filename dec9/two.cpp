//usr/bin/g++ -g -O2 dec9/two.cpp -o dec9/.two && time ./dec9/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


struct Marble
{
	size_t value;
	size_t prev;
	size_t next;
};

size_t prev(std::vector<Marble>& marbles, size_t cur)
{
	return marbles[cur].prev;
}

size_t next(std::vector<Marble>& marbles, size_t cur)
{
	return marbles[cur].next;
}

size_t insert(std::vector<Marble>& marbles, size_t cur, size_t value)
{
	marbles[value].value = value;
	marbles[value].prev = marbles[cur].prev;
	marbles[value].next = cur;

	marbles[marbles[cur].prev].next = value;
	marbles[cur].prev = value;

	return value;
}

size_t erase(std::vector<Marble>& marbles, size_t cur)
{
	size_t value = marbles[cur].value;

	marbles[marbles[cur].next].prev = marbles[cur].prev;
	marbles[marbles[cur].prev].next = marbles[cur].next;

	return marbles[cur].next;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec9/input.txt");

	std::string line;
	std::getline(file, line);

	int players, maxvalue;
	sscanf(line.c_str(), "%d players; last marble is worth %d points",
		&players, &maxvalue);

	std::vector<int> scores(players, 0);

	std::vector<Marble> marbles;
	marbles.resize(maxvalue + 1);
	marbles[0].value = 0;
	marbles[0].prev = 0;
	marbles[0].next = 0;
	size_t cur = 0;

	for (int value = 1; value <= maxvalue; value++)
	{
		if (value % 23 == 0)
		{
			for (int i = 0; i < 7; i++)
			{
				cur = prev(marbles, cur);
			}

			int player = value % players;
			scores[player] += value;
			scores[player] += marbles[cur].value;
			cur = erase(marbles, cur);
		}
		else
		{
			cur = next(marbles, cur);
			cur = next(marbles, cur);
			cur = insert(marbles, cur, value);
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

// Finally, a Big-Oh challenge.
