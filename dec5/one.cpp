//usr/bin/g++ dec5/one.cpp -o dec5/.one && time ./dec5/.one; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cassert>

bool match(char x, char y)
{
	return ((x & 0x20) != (y & 0x20) && (x & 0x1F) == (y & 0x1F));
}

int main(int /*argc*/, char* /*argv*/[])
{
	// Unit tests.
	assert(match('a', 'A'));
	assert(match('A', 'a'));
	assert(match('z', 'Z'));
	assert(!match('a', 'a'));
	assert(!match('Z', 'Z'));
	assert(!match('a', 'b'));
	assert(!match('a', 'z'));
	assert(!match('A', 'B'));
	assert(!match('A', 'Z'));

	std::ifstream file("dec5/input.txt");

	std::string line;
	std::getline(file, line);

	std::vector<std::pair<size_t, size_t>> cuts;
	size_t end = 1;
	size_t cur = 1;

	while (cur < line.size())
	{
		//std::cout << end << ":" << cur;
		//{
		//	std::cout << " \t [";
		//	size_t i = 0;
		//	for (const auto& cut : cuts)
		//	{
		//		std::cout << line.substr(i, cut.first - i)
		//			<< std::string(cut.second - cut.first, '-');
		//		i = cut.second;
		//	}
		//	std::cout << line.substr(i) << "]" << std::endl;
		//}

		if (end == 0)
		{
			cur++;
			end = cur;
		}
		else if (match(line[end - 1], line[cur]))
		{
			if (cuts.empty())
			{
				cuts.emplace_back(end - 1, cur + 1);
				end--;
				cur++;
			}
			else if (cuts.back().second < end - 1)
			{
				cuts.emplace_back(end - 1, cur + 1);
				end--;
				cur++;
			}
			else if (cuts.back().second == end - 1)
			{
				end = cuts.back().first;
				cuts.back().second = cur + 1;
				cur++;
			}
			else
			{
				cuts.back().first = end - 1;
				cuts.back().second = cur + 1;
				end--;
				cur++;
			}
		}
		else
		{
			cur++;
			end = cur;
		}
	}

	//{
	//	std::cout << "Result:\t [";
	//	size_t i = 0;
	//	for (const auto& cut : cuts)
	//	{
	//		std::cout << line.substr(i, cut.first - i)
	//			<< std::string(cut.second - cut.first, '-');
	//		i = cut.second;
	//	}
	//	std::cout << line.substr(i) << "]" << std::endl;
	//}

	size_t remaining = line.size();
	for (const auto& cut : cuts)
	{
		remaining -= (cut.second - cut.first);
	}
	std::cout << remaining << std::endl;
}

/*
dDbAccCaACBAcCcaDAad
  bAccCaACBAcCcaDAad
  bAc  aACBAcCcaDAad
  bAc    CBAcCcaDAad
  bA      BAcCcaDAad
  bA      BA  caDAad
  bA      BA  caD  d
  bA      BA  ca    /
*/
