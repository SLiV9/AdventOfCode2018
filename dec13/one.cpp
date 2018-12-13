//usr/bin/g++ -g -O2 dec13/one.cpp -o dec13/.one && time ./dec13/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>

#define STATELEFT 0
#define STATESTRAIGHT 1
#define STATERIGHT 2
#define STATES 3

struct Cart
{
	int xenon;
	int yahoo;
	int hspd;
	int vspd;
	uint8_t state;

	Cart(int x, int y, int h, int v) :
		xenon(x),
		yahoo(y),
		hspd(h),
		vspd(v),
		state(STATELEFT)
	{}
};

constexpr bool operator<(const Cart& a, const Cart& b)
{
	return (a.yahoo < b.yahoo) || (a.yahoo == b.yahoo && a.xenon << b.xenon);
}

void print(const std::vector<std::vector<char>>& board,
	const std::vector<Cart>& carts)
{
	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			char c = board[y][x];
			for (const Cart& cart : carts)
			{
				if (cart.xenon == x && cart.yahoo == y)
				{
					if      (cart.hspd > 0) c = '>';
					else if (cart.hspd < 0) c = '<';
					else if (cart.vspd > 0) c = 'v';
					else if (cart.vspd < 0) c = '^';
					else                    c = 'X';
				}
			}
			std::cout << c;
		}
		std::cout << std::endl;
	}
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec13/input.txt");

	std::vector<std::vector<char>> board;
	std::vector<Cart> carts;

	std::string line;
	while (std::getline(file, line))
	{
		board.emplace_back();

		std::stringstream strm(line);
		for (char x : line)
		{
			switch (x)
			{
				case '^':
				case 'v':
				{
					carts.emplace_back(board.back().size(), board.size() - 1,
						0, (x == 'v') - (x == '^'));
					board.back().emplace_back('|');
				}
				break;

				case '>':
				case '<':
				{
					carts.emplace_back(board.back().size(), board.size() - 1,
						(x == '>') - (x == '<'), 0);
					board.back().emplace_back('-');
				}
				break;

				default:
				{
					board.back().emplace_back(x);
				}
				break;
			}
		}

		if (board.size() > 1)
		{
			if (board.back().size() > board[0].size())
			{
				for (std::vector<char>& row : board)
				{
					row.resize(board.back().size(), ' ');
				}
			}
			else if (board.back().size() < board[0].size())
			{
				board.back().resize(board[0].size(), ' ');
			}
		}
	}

	bool crashed = false;
	int crashx = -1;
	int crashy = -1;
	size_t updatetick = 1;
	for (size_t tick = 0; !crashed; tick++)
	{
		if (tick == updatetick)
		{
			std::cout << "After " << tick << " ticks:" << std::endl;
			print(board, carts);
			updatetick *= 2;
		}

		std::sort(carts.begin(), carts.end());
		for (size_t i = 0; i < carts.size(); i++)
		{
			Cart& cart = carts[i];
			int x = cart.xenon + cart.hspd;
			int y = cart.yahoo + cart.vspd;

			for (size_t j = 0; j < carts.size(); j++)
			{
				if (i != j
					&& x == carts[j].xenon
					&& y == carts[j].yahoo
					&& cart.hspd == -carts[j].hspd
					&& cart.vspd == -carts[j].vspd)
				{
					std::cout << "About to crash"
						" after " << tick << " ticks!" << std::endl;
					print(board, carts);

					cart.hspd = 0;
					cart.vspd = 0;
					carts[j].hspd = 0;
					carts[j].vspd = 0;
					crashed = true;
					crashx = x;
					crashy = y;
				}
			}

			cart.xenon = x;
			cart.yahoo = y;

			if (crashed)
			{
				break;
			}

			switch (board[y][x])
			{

				case '/':
				{
					int h = -cart.vspd;
					int v = -cart.hspd;
					cart.hspd = h;
					cart.vspd = v;
				}
				break;

				case '\\':
				{
					int h = cart.vspd;
					int v = cart.hspd;
					cart.hspd = h;
					cart.vspd = v;
				}
				break;

				case '+':
				{
					switch (cart.state)
					{
						case STATELEFT:
						{
							int h = cart.vspd;
							int v = -cart.hspd;
							cart.hspd = h;
							cart.vspd = v;
						}
						break;

						case STATESTRAIGHT:
						break;

						case STATERIGHT:
						{
							int h = -cart.vspd;
							int v = cart.hspd;
							cart.hspd = h;
							cart.vspd = v;
						}
						break;
					}

					cart.state = (cart.state + 1) % STATES;
				}
				break;

				default:
				break;
			}
		}
	}

	if (crashed)
	{
		std::cout << "Crashed!" << std::endl;
	}
	else
	{
		std::cout << "No crash!" << std::endl;
	}

	print(board, carts);

	std::cout << "Crash at " << crashx << "," << crashy << std::endl;
}
