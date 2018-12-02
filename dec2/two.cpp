//usr/bin/g++ dec2/two.cpp -o dec2/.two; ./dec2/.two; exit

#include <vector>
#include <iostream>
#include <fstream>


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec2/input.txt");

	std::vector<std::string> words;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;
		words.emplace_back(line);
	}

	for (size_t i = 0; i < words.size(); i++)
	{
		for (size_t j = 0; j < i; j++)
		{
			size_t diffpos = size_t(-1);
			for (size_t p = 0; p < words[i].size() && p < words[j].size(); p++)
			{
				if (words[i][p] == words[j][p])
				{
					// continue;
				}
				else if (diffpos == size_t(-1))
				{
					diffpos = p;
				}
				else
				{
					diffpos = size_t(-1);
					break;
				}
			}

			if (diffpos != size_t(-1))
			{
				std::cout << words[i].substr(0, diffpos)
					<< words[i].substr(diffpos + 1)
					<< std::endl;
			}
		}
	}
}
