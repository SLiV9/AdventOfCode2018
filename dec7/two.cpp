//usr/bin/g++ -O2 dec7/two.cpp -o dec7/.two && time ./dec7/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cassert>


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec7/input.txt");

	uint8_t size = 0;
	std::vector<std::pair<uint8_t, uint8_t>> dependencies;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		char x, y;
		sscanf(line.c_str(), "Step %c must be finished before step %c", &x, &y);

		uint8_t i = x - 'A';
		uint8_t j = y - 'A';

		if (i + 1 > size) size = i + 1;
		if (j + 1 > size) size = j + 1;

		// j has dependency i
		dependencies.emplace_back(j, i);
	}

	std::cout << std::to_string(size) << std::endl;

	uint8_t mintime = (size > 10) ? 60 : 0;
	uint8_t workers = (size > 10) ? 5 : 2;

	std::sort(dependencies.begin(), dependencies.end());

	std::vector<bool> begun;
	begun.resize(size, false);

	std::vector<uint8_t> worktime;
	worktime.resize(workers, 0);
	std::vector<uint8_t> work;
	work.resize(workers, 0);

	uint8_t completed = 0;
	size_t t = 0;
	for (; completed < size && t < size_t(size) * (mintime + size); t++)
	{
		for (uint8_t w = 0; w < workers; w++)
		{
			if (worktime[w] > 0)
			{
				continue;
			}

			//std::cout << std::endl << char('1' + w) << " looking ";

			for (uint8_t i = 0; i < size; i++)
			{
				if (begun[i])
				{
					continue;
				}

				{
					auto lb = std::lower_bound(dependencies.begin(),
						dependencies.end(),
						std::pair<uint8_t, uint8_t>(i, 0));

					if (lb != dependencies.end() && lb->first == i)
					{
						continue;
					}
				}

				//std::cout << std::endl << char('1' + w) << " started ";
				//std::cout << char('A' + i);

				begun[i] = true;
				work[w] = i;
				worktime[w] = mintime + i + 1;
				break;
			}
		}

		for (uint8_t w = 0; w < workers; w++)
		{
			if (worktime[w] == 0)
			{
				continue;
			}

			//std::cout << std::endl << char('1' + w) << " working ";

			worktime[w]--;

			if (worktime[w] == 0)
			{
				//std::cout << std::endl << char('1' + w) << " finished ";

				uint8_t i = work[w];

				dependencies.erase(
					std::remove_if(dependencies.begin(), dependencies.end(),
						[i](const std::pair<uint8_t, uint8_t>& dep) {
							return dep.second == i;
						}),
					dependencies.end());

				completed++;
				std::cout << char('A' + i);
			}
		}
	}
	std::cout << std::endl;

	std::cout << "Took " << t << " seconds" << std::endl;
}

// The instruction "If more than one step is ready, choose the step which is
// first alphabetically." hides a clue that I can just use the alphabeticality
// of the steps; first try if A is possible, then B, then C, until you get a
// hit, and then go back and try A again.
