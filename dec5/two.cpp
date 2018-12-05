//usr/bin/g++ dec5/two.cpp -o dec5/.two && time ./dec5/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cassert>

bool similar(char x, char y)
{
	return (x & 0x1F) == (y & 0x1F);
}

bool match(char x, char y)
{
	return ((x & 0x20) != (y & 0x20) && (x & 0x1F) == (y & 0x1F));
}

size_t reduce(const std::string& line, char ignored);

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec5/input.txt");
	std::string line;
	std::getline(file, line);

	char bestchar = 0;
	size_t bestremaining;
	for (char x = 'a'; x <= 'z'; x++)
	{
		size_t remaining = reduce(line, x);
		if (bestchar == 0 || remaining < bestremaining)
		{
			bestchar = x;
			bestremaining = remaining;
		}
	}

	std::cout << "Best char '" << bestchar << "'"
		<< " with " << bestremaining << " remaining" << std::endl;
}

size_t reduce(const std::string& line, char ignored)
{
	std::vector<std::pair<size_t, size_t>> cuts;
	size_t end = 0;
	size_t cur = 0;

	while (cur <= line.size())
	{
		/*
		std::cout << end << ":" << cur;
		{
			size_t i = (size_t) std::max(int(end - 3), 0);
			size_t j = std::min(cur + 3, line.size());
			std::cout << " \t " << i << " \t ["
				<< std::string(std::min(size_t(3), i), ' ');
			for (const auto& cut : cuts)
			{
				if (cut.second < i) continue;
				if (cut.first < i)
				{
					std::cout << std::string(cut.second - i, '-');
				}
				else
				{
					std::cout << line.substr(i, cut.first - i)
						<< std::string(cut.second - cut.first, '-');
				}
				i = cut.second;
				if (i > j) break;
			}
			std::cout << line.substr(i, j - i)
				<< std::string(std::min(size_t(3), j - i), ' ')
				<< "]" << std::endl;
		}
		*/

		if (end == 0)
		{
			cur++;
			end = cur;
		}
		else if (similar(line[end - 1], ignored))
		{
			if (cuts.empty() || cuts.back().second < end - 1)
			{
				cuts.emplace_back(end - 1, cur);
				end--;
			}
			else if (cuts.back().second == end - 1)
			{
				end = cuts.back().first;
				cuts.back().second = cur;
			}
			else if (cuts.size() < 2 || cuts[cuts.size() - 2].second < end - 1)
			{
				cuts.back().first = end - 1;
				cuts.back().second = cur;
				end--;
			}
			else
			{
				cuts.pop_back();
				end = cuts.back().first;
				cuts.back().second = cur;
			}
		}
		else if (cur >= line.size())
		{
			break;
		}
		else if (match(line[end - 1], line[cur]))
		{
			if (cuts.empty() || cuts.back().second < end - 1)
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
			else if (cuts.size() < 2 || cuts[cuts.size() - 2].second < end - 1)
			{
				cuts.back().first = end - 1;
				cuts.back().second = cur + 1;
				end--;
				cur++;
			}
			else
			{
				cuts.pop_back();
				end = cuts.back().first;
				cuts.back().second = cur + 1;
				cur++;
			}
		}
		else
		{
			cur++;
			end = cur;
		}
	}

	/*
	{
		std::cout << "Result";
		size_t i = (size_t) std::max(int(end - 3), 0);
		size_t j = std::min(cur + 3, line.size());
		std::cout << " \t ["
			<< std::string(std::min(size_t(3), i), ' ');
		for (const auto& cut : cuts)
		{
			if (cut.second < i) continue;
			if (cut.first < i)
			{
				std::cout << std::string(cut.second - i, '-');
			}
			else
			{
				std::cout << line.substr(i, cut.first - i)
					<< std::string(cut.second - cut.first, '-');
			}
			i = cut.second;
			if (i > j) break;
		}
		std::cout << line.substr(i, j - i)
			<< std::string(std::min(size_t(3), j - i), ' ')
			<< "]" << std::endl;
	}
	*/

	size_t remaining = line.size();
	for (const auto& cut : cuts)
	{
		remaining -= (cut.second - cut.first);
	}

	std::cout << ignored << ": " << remaining << std::endl << std::endl;

	return remaining;
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
