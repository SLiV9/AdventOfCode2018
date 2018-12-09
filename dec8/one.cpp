//usr/bin/g++ -g -O2 dec8/one.cpp -o dec8/.one && time ./dec8/.one; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


static int _stored = -1;

void store(int x)
{
	_stored = x;
}

bool retrieve(int* x)
{
	if (_stored < 0) return false;

	*x = _stored;
	_stored = -1;
	return true;
}


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec8/input.txt");

	std::string line;
	std::getline(file, line);
	std::stringstream strm(line);

	std::vector<std::pair<int, int>> stack;

	int sum = 0;

	int x;
	while (retrieve(&x) || strm >> x)
	{
		if (stack.empty())
		{
			stack.emplace_back(x, -1);
		}
		else if (stack.back().second < 0)
		{
			stack.back().second = x;
		}
		else if (stack.back().first > 0)
		{
			stack.back().first--;
			stack.emplace_back(x, -1);
		}
		else if (stack.back().second > 0)
		{
			stack.back().second--;
			sum += x;
		}
		else
		{
			stack.pop_back();
			store(x);
		}
	}

	std::cout << sum << std::endl;
}
