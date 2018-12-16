//usr/bin/g++ -g -O2 dec15/two.cpp -o dec15/.two && time ./dec15/.two; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


constexpr char enemy(char t)
{
	return ('E' + 'G' - t);
}

struct Position
{
	uint8_t xenon;
	uint8_t yahoo;

	Position(uint8_t x, uint8_t y) :
		xenon(x),
		yahoo(y)
	{}

	constexpr bool isAdjacentTo(const Position& other)
	{
		return ((int(xenon) - other.xenon) * (int(xenon) - other.xenon)
			+   (int(yahoo) - other.yahoo) * (int(yahoo) - other.yahoo) == 1);
	}
};

struct Unit : public Position
{
	char type;
	uint8_t hitpoints;

	Unit(char t, uint8_t x, uint8_t y) :
		Position(x, y),
		type(t),
		hitpoints(200)
	{}
};

constexpr bool operator==(const Position& a, const Position& b)
{
	return (a.yahoo == b.yahoo && a.xenon == b.xenon);
}

constexpr bool operator<(const Position& a, const Position& b)
{
	return (a.yahoo < b.yahoo) || (a.yahoo == b.yahoo && a.xenon < b.xenon);
}

constexpr bool operator<(const Position& a, uint8_t y)
{
	return (a.yahoo < y);
}

void print(const std::vector<std::vector<char>>& board,
	const std::vector<Unit>& units)
{
	// Pre-condition: units is sorted.
	auto unit = units.begin();

	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			std::cout << board[y][x];
		}
		const char* sep = "   ";
		while (unit != units.end() && unit->yahoo < y) { ++unit; }
		for (; unit != units.end() && unit->yahoo == y; ++unit)
		{
			std::cout << sep;
			std::cout << unit->type << "(" << int(unit->hitpoints) << ")";
			sep = ", ";
		}
		std::cout << std::endl;
	}
}

int fill(/*mutable*/ std::vector<std::vector<char>>& board,
	const std::vector<Unit>& units)
{
	for (const Unit& unit : units)
	{
		board[unit.yahoo][unit.xenon] = unit.type;
	}
}

int clean(/*mutable*/ std::vector<std::vector<char>>& board,
	const std::vector<Unit>& units)
{
	for (const Unit& unit : units)
	{
		board[unit.yahoo][unit.xenon] = '.';
	}
}

bool simulate(/*mutable*/ std::vector<std::vector<char>>& board,
	/*mutable*/ std::vector<std::vector<uint8_t>>& hmap,
	/*mutable*/ std::vector<Unit>& units,
	uint8_t elfdamage);

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec15/input.txt");

	std::vector<std::vector<char>> board;
	std::vector<std::vector<uint8_t>> hmap;
	std::vector<Unit> units;

	std::string line;
	while (std::getline(file, line))
	{
		board.emplace_back();

		std::stringstream strm(line);
		for (char x : line)
		{
			if (x == 'E' || x == 'G')
			{
				units.emplace_back(x, board.back().size(), board.size() - 1);
			}

			board.back().emplace_back(x);
		}

		hmap.emplace_back(board.back().size());
	}

	uint8_t mindamage = 4;
	uint8_t maxdamage = 200;
	uint8_t elfdamage = 20;
	while (mindamage < maxdamage)
	{
		std::cout << int(mindamage) << " <= " << int(elfdamage)
			<< " <= " << int(maxdamage) << std::endl;
		std::vector<Unit> simulants = units;
		bool survival = simulate(board, hmap, simulants, elfdamage);
		clean(board, simulants);
		fill(board, units);

		if (survival)
		{
			maxdamage = elfdamage;
			if (elfdamage > mindamage)
			{
				// Take the middle of the half-open [mindamage, elfdamage).
				elfdamage = mindamage + (elfdamage - mindamage) / 2;
			}
		}
		else
		{
			mindamage = elfdamage + 1;
			if (elfdamage < maxdamage)
			{
				// Take the middle of the half-open (elfdamage, maxdamage].
				elfdamage = maxdamage - (maxdamage - elfdamage) / 2;
			}
		}
	}
}

bool simulate(/*mutable*/ std::vector<std::vector<char>>& board,
	/*mutable*/ std::vector<std::vector<uint8_t>>& hmap,
	/*mutable*/ std::vector<Unit>& units,
	uint8_t elfdamage)
{
	std::cout << "Elves deal " << int(elfdamage) << " damage" << std::endl;

	size_t rounds = 0;
	bool combat = true;
	while (combat)
	{
		//std::cout << "After " << rounds << " rounds:" << std::endl;
		//print(board, units);

		for (Unit& activeunit : units)
		{
			if (activeunit.hitpoints == 0) continue;

			for (std::vector<uint8_t>& row : hmap)
			{
				row.assign(row.size(), uint8_t(-1));
			}

			std::vector<Position> queue;
			std::vector<Position> nextqueue;
			queue.emplace_back(activeunit.xenon, activeunit.yahoo);

			bool found = false;
			Position targetpos(0, 0);
			uint8_t distance = 0;
			while (!queue.empty() && !found && distance < uint8_t(-1))
			{
				std::sort(queue.begin(), queue.end());
				queue.erase(std::unique(queue.begin(), queue.end()),
					queue.end());

				for (const Position& pos : queue)
				{
					uint8_t x = pos.xenon;
					uint8_t y = pos.yahoo;

					hmap[y][x] = distance;
					if (board[y][x] == enemy(activeunit.type))
					{
						found = true;
						targetpos = pos;
						break;
					}

					// Making use of the fact that the maps we are given are
					// nice and surrounded by #'s on all sides.
					for (const Position& to : {
							Position{x, uint8_t(y-1)},
							Position{uint8_t(x-1), y},
							Position{uint8_t(x+1), y},
							Position{x, uint8_t(y+1)},
						})
					{
						if (board[to.yahoo][to.xenon] != '#'
							&& board[to.yahoo][to.xenon] != activeunit.type
							&& hmap[to.yahoo][to.xenon] > distance)
						{
							nextqueue.emplace_back(to);
						}
					}
				}

				queue.clear();
				if (found)
				{
					nextqueue.clear();
				}
				else
				{
					nextqueue.swap(queue);
					distance++;
				}
			}

			if (!found)
			{
				bool any = false;
				for (const Unit& otherunit : units)
				{
					if (otherunit.type == enemy(activeunit.type)
						&& otherunit.hitpoints > 0)
					{
						any = true;
						break;
					}
				}

				if (any) continue;
				else
				{
					combat = false;
					break;
				}
			}

			//for (size_t y = 0; y < hmap.size(); y++)
			//{
			//	for (size_t x = 0; x < hmap[y].size(); x++)
			//	{
			//		if (hmap[y][x] == uint8_t(-1)) std::cout << '-';
			//		else std::cout << (int(hmap[y][x]) % 10);
			//	}
			//	std::cout << std::endl;
			//}

			if (distance >= 2)
			{
				queue.emplace_back(targetpos);
				for (uint8_t d = distance; d > 0; d--)
				{
					std::sort(queue.begin(), queue.end());
					queue.erase(std::unique(queue.begin(), queue.end()),
						queue.end());

					for (const Position& pos : queue)
					{
						//std::cout
						//	<< "queue >> " << int(pos.xenon)
						//	<< "," << int(pos.yahoo)
						//	<< std::endl;
						uint8_t x = pos.xenon;
						uint8_t y = pos.yahoo;

						if (d == 1)
						{
							//std::cout
							//	<< "from " << int(activeunit.xenon)
							//	<< "," << int(activeunit.yahoo)
							//	<< " to " << int(x) << "," << int(y)
							//	<< std::endl;

							board[y][x] = activeunit.type;
							board[activeunit.yahoo][activeunit.xenon] = '.';
							activeunit.xenon = x;
							activeunit.yahoo = y;
							break;
						}

						// Making use of the fact that the maps we are given are
						// nice and surrounded by #'s on all sides.
						for (const Position& to : {
								Position{x, uint8_t(y-1)},
								Position{uint8_t(x-1), y},
								Position{uint8_t(x+1), y},
								Position{x, uint8_t(y+1)},
							})
						{
							if (hmap[to.yahoo][to.xenon] < d)
							{
								//std::cout
								//	<< "nextqueue << " << int(to.xenon)
								//	<< "," << int(to.yahoo)
								//	<< std::endl;
								nextqueue.emplace_back(to);
							}
						}
					}

					queue.clear();
					nextqueue.swap(queue);
				}
			}

			if (distance <= 2)
			{
				// Find the weakest adjacent enemy unit.
				Unit* target = nullptr;
				uint8_t weakest = uint8_t(-1);
				for (Unit& otherunit : units)
				{
					if (!otherunit.isAdjacentTo(activeunit)) continue;
					if (otherunit.type == activeunit.type) continue;
					if (otherunit.hitpoints == 0) continue;
					if (otherunit.hitpoints >= weakest) continue;

					target = &otherunit;
					weakest = otherunit.hitpoints;
				}

				if (target != nullptr)
				{
					uint8_t damage = (activeunit.type == 'E') ? elfdamage : 3;

					// Check lethality first to prevent underflow.
					if (target->hitpoints <= damage)
					{
						target->hitpoints = 0;
						board[target->yahoo][target->xenon] = '.';
					}
					else
					{
						target->hitpoints -= damage;
					}
				}
			}

			//print(board, units);
		}

		units.erase(std::remove_if(units.begin(), units.end(),
			[](const Unit& unit){ return (unit.hitpoints == 0);	}),
			units.end());
		std::sort(units.begin(), units.end());

		if (combat)
		{
			rounds++;
		}
	}

	bool victory = (!units.empty() && units[0].type == 'E');
	if (victory)
	{
		std::cout << "Elves are victorious!" << std::endl;
	}
	else
	{
		std::cout << "Elves are defeated" << std::endl;
	}
	print(board, units);

	std::cout << "Completed " << rounds << " full rounds" << std::endl;
	int sum = 0;
	for (const Unit& unit : units)
	{
		sum += unit.hitpoints;
	}
	std::cout << "x" << sum << "  ==>  " << (rounds * sum) << std::endl;

	return victory;
}
