//usr/bin/g++ -O2 dec7/one.cpp -o dec7/.one && time ./dec7/.one; exit

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

	std::sort(dependencies.begin(), dependencies.end());
	std::vector<uint8_t> found;
	found.reserve(size);

	for (uint8_t f = 0; f < size; f++)
	{
		for (uint8_t i = 0; i < size; i++)
		{
			if (std::find(found.begin(), found.end(), i) != found.end())
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

			dependencies.erase(
				std::remove_if(dependencies.begin(), dependencies.end(),
					[i](const std::pair<uint8_t, uint8_t>& dep) {
						return dep.second == i;
					}),
				dependencies.end());

			found.emplace_back(i);
			std::cout << char('A' + i);
			break;
		}
	}
	std::cout << std::endl;
}

// The instruction "If more than one step is ready, choose the step which is
// first alphabetically." hides a clue that I can just use the alphabeticality
// of the steps; first try if A is possible, then B, then C, until you get a
// hit, and then go back and try A again.
