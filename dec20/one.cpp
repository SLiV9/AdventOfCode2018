//usr/bin/g++ -g -O2 dec20/one.cpp -o dec20/.one && time ./dec20/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


struct Probe
{
	int x;
	int y;

	constexpr bool operator==(const Probe& other)
	{
		return (x == other.x && y == other.y);
	}

	constexpr bool operator<(const Probe& other)
	{
		return (y < other.y || (y == other.y && x < other.x));
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	std::string line;
	{
		std::ifstream file("dec20/test8.txt");
		std::getline(file, line);
	}

	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	{
		std::vector<Probe> probes = { {0, 0} };
		std::vector<size_t> stack = { 0 };
		for (char c : line)
		{
			switch (c)
			{
				case '^':
				break;
				case 'E':
				{
					for (size_t i = stack.back(); i < probes.size(); i++)
					{
						int& x = probes[i].x;
						x++;
						if (x > maxx) maxx = x;
					}
				}
				break;
				case 'S':
				{
					for (size_t i = stack.back(); i < probes.size(); i++)
					{
						int& y = probes[i].y;
						y++;
						if (y > maxy) maxy = y;
					}
				}
				break;
				case 'W':
				{
					for (size_t i = stack.back(); i < probes.size(); i++)
					{
						int& x = probes[i].x;
						x--;
						if (x < minx) minx = x;
					}
				}
				break;
				case 'N':
				{
					for (size_t i = stack.back(); i < probes.size(); i++)
					{
						int& y = probes[i].y;
						y--;
						if (y < miny) miny = y;
					}
				}
				break;
				case '(':
				{
					size_t start = stack.back();
					size_t end = probes.size();
					size_t len = end - start;

					size_t cur = probes.size();
					probes.resize(cur + len);
					std::copy_n(probes.begin() + start, len,
						probes.begin() + cur);

					stack.push_back(end);
					stack.push_back(cur);
				}
				break;
				case '|':
				{
					stack.pop_back();
					size_t end = stack.back();
					stack.pop_back();
					size_t start = stack.back();
					size_t len = end - start;

					size_t cur = probes.size();
					probes.resize(cur + len);
					std::copy_n(probes.begin() + start, len,
						probes.begin() + cur);

					stack.push_back(end);
					stack.push_back(cur);
				}
				break;
				case ')':
				{
					stack.pop_back();
					size_t end = stack.back();
					stack.pop_back();
					size_t start = stack.back();

					probes.erase(probes.begin() + start, probes.begin() + end);

					std::sort(probes.begin() + start, probes.end());
					probes.erase(
						std::unique(probes.begin() + start, probes.end()),
						probes.end());
				}
				break;
				case '$':
				break;
			}

			std::cout << c << "   ";
			//size_t s = 0;
			//const char* sep = "";
			//for (size_t i = 0; i < probes.size(); i++)
			//{
			//	std::cout << sep;
			//	while (s < stack.size() && stack[s] == i)
			//	{
			//		std::cout << s << ": ";
			//		s++;
			//	}
			//	std::cout << probes[i].x << "," << probes[i].y;
			//	sep = "; ";
			//}
			std::cout << "   (" << probes.size() << ")" << std::endl;
		}
	}

	std::cout << "x from " << minx << " to " << maxx << std::endl;
	std::cout << "y from " << miny << " to " << maxy << std::endl;

	//std::cout << "The furthest room requires passing "
	//	<< maxlen << " doors" << std::endl;
}


// ^E(E|N(SS(WN|)S|WSE(SW|NE|))E)$
/*
^   0: 0,0
E   0: 1,0
(   0: 1,0; 1: 2: 1,0
E   0: 1,0; 1: 2: 2,0
|   0: 1,0; 1: 2,0; 2: 1,0
N   0: 1,0; 1: 2,0; 2: 1,-1
(   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,-1
S   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,0
S   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,1
(   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,1; 5: 6: 1,1
W   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,1; 5: 6: 0,1
N   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,1; 5: 6: 0,0
|   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 1,1; 5: 0,0; 6: 1,1
)   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 0,0; 1,1
S   0: 1,0; 1: 2,0; 2: 1,-1; 3: 4: 0,1; 1,2
|   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,-1
W   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 0,-1
S   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 0,0
E   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0
(   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 6: 1,0
S   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 6: 1,1
W   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 6: 0,1
|   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 0,1; 6: 1,0
N   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 0,1; 6: 1,-1
E   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 0,1; 6: 2,-1
|   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 1,0; 5: 0,1; 2,-1; 6: 1,0
)   0: 1,0; 1: 2,0; 2: 1,-1; 3: 0,1; 1,2; 4: 0,1; 2,-1; 1,0
)   0: 1,0; 1: 2,0; 2: 0,1; 1,2; 0,1; 2,-1; 1,0
E   0: 1,0; 1: 2,0; 2: 1,1; 2,2; 1,1; 3,-1; 2,0
)   0: 2,0; 1,1; 2,2; 1,1; 3,-1; 2,0
$   0: 2,0; 1,1; 2,2; 1,1; 3,-1; 2,0
*/
