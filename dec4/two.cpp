//usr/bin/g++ dec4/two.cpp -o dec4/.two && ./dec4/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec4/input.txt");

	std::vector<std::string> lines;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		// [1518-03-08 23:56] Guard #3511 begins shift
		// [1518-
		lines.push_back(line.substr(6));
	}

	std::sort(lines.begin(), lines.end());

	std::vector<int> guards;
	std::vector<std::vector<uint8_t>> sleeps;

	size_t sleepoffset = 0;
	int guard = -1;
	int from = -1;
	int to = -1;

	for (const std::string& line : lines)
	{
		// 03-08 23:56] Guard #3511 begins shift
		// 03-08 23:56] G
		if (line[13] == 'G')
		{
			// 03-08 23:56] Guard #
			sscanf(line.c_str() + 20, "%d ", &guard);

			to = -1;

			bool found = false;
			for (size_t i = 0; i < guards.size(); i++)
			{
				if (guards[i] == guard)
				{
					found = true;
					sleepoffset = i;
				}
			}

			if (!found)
			{
				sleepoffset = sleeps.size();
				guards.emplace_back(guard);
				sleeps.emplace_back(60, 0);
			}
		}
		// 05-30 00:48] falls asleep
		// 05-30 00:48] f
		else if (line[13] == 'f')
		{
			if (guard < 0)
			{
				std::cerr << "No guard" << std::endl;
				return -1;
			}

			// 07-30 00:
			if (sscanf(line.c_str() + 9, "%d", &from) < 1)
			{
				std::cerr << "No from: '" << line << "'" << std::endl;
				return -1;
			}
			else if (from < 0)
			{
				std::cerr << "Bad from: '" << line << "'" << std::endl;
				return -1;
			}
			else if (to >= 0 && from <= to)
			{
				std::cerr << "Missing guard" << std::endl;
				return -1;
			}
		}
		// 07-30 00:30] wakes up
		// 07-30 00:30] w
		else if (line[13] == 'w')
		{
			if (guard < 0)
			{
				std::cerr << "No guard" << std::endl;
				return -1;
			}

			// 07-30 00:
			if (sscanf(line.c_str() + 9, "%d", &to) < 1)
			{
				std::cerr << "No to: '" << line << "'" << std::endl;
				return -1;
			}
			else if (to < 0)
			{
				std::cerr << "Bad to: '" << line << "'" << std::endl;
				return -1;
			}
			else if (from < 0 || to <= from)
			{
				std::cerr << "Bad sleep" << std::endl;
				return -1;
			}

			for (size_t t = size_t(from); t < size_t(to); t++)
			{
				sleeps[sleepoffset][t] += 1;
			}
		}
		else
		{
			std::cerr << "Bad data: '" << line << "'" << std::endl;
			return -1;
		}
	}

	int best = -1;
	int bestmax;
	int bestmin; // hehe
	int bestid;
	for (size_t i = 0; i < guards.size(); i++)
	{
		std::cout << "#" << guards[i] << ":\t";
		for (size_t t = 0; t < 60; t++)
		{
			std::cout << " " << std::to_string(sleeps[i][t]);
		}
		std::cout << std::endl;

		int max = -1;
		int min; // hehe
		for (size_t t = 0; t < 60; t++)
		{
			int x = int(sleeps[i][t]);
			if (max < x)
			{
				max = x;
				min = t;
			}
		}

		if (best < 0 || bestmax < max)
		{
			best = i;
			bestmax = max;
			bestmin = min;
			bestid = guards[i];
		}
	}

	std::cout << "#" << bestid << " x t=" << bestmin
		<< " == " << bestid * bestmin << std::endl;
}
