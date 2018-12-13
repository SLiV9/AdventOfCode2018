//usr/bin/g++ -g -O2 dec13/two.cpp -o dec13/.two && time ./dec13/.two; exit

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

#define DESTROYED 99

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
	return (a.yahoo < b.yahoo) || (a.yahoo == b.yahoo && a.xenon < b.xenon);
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
				if (cart.state == DESTROYED) continue;
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

	size_t updatetick = 1;
	for (size_t tick = 0; carts.size() > 1; tick++)
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
			if (cart.state == DESTROYED) continue;

			int x = cart.xenon + cart.hspd;
			int y = cart.yahoo + cart.vspd;

			bool crashed = false;
			for (size_t j = 0; j < carts.size(); j++)
			{
				if (i != j
					&& carts[j].state != DESTROYED
					&& x == carts[j].xenon
					&& y == carts[j].yahoo)
				{
					std::cout << "About to crash"
						" after " << tick << " ticks!" << std::endl;
					print(board, carts);

					cart.state = DESTROYED;
					carts[j].state = DESTROYED;
					crashed = true;
				}
			}

			cart.xenon = x;
			cart.yahoo = y;

			if (crashed)
			{
				std::cout << "Crashed!" << std::endl;
				print(board, carts);
				continue;
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

		carts.erase(std::remove_if(carts.begin(), carts.end(),
			[](const Cart& cart){
				return cart.state == DESTROYED;
			}),
			carts.end());
	}

	int cartx = -1;
	int carty = -1;
	if (carts.size() == 1)
	{
		cartx = carts[0].xenon;
		carty = carts[0].yahoo;
		std::cout << "Only one cart remaining." << std::endl;
	}
	else
	{
		std::cout << "No carts remaining." << std::endl;
	}

	print(board, carts);

	std::cout << "Last cart at " << cartx << "," << carty << std::endl;
}
