//usr/bin/g++ -O2 dec6/two.cpp -o dec6/.two && time ./dec6/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cassert>

#define WHITE 255
#define BLACK 254

constexpr uint8_t paint(uint8_t paper, uint8_t ink)
{
	return (paper == WHITE) ? ink : BLACK;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec6/input.txt");

	std::vector<std::pair<int, int>> nodes;
	int minx = -1;
	int miny = -1;
	int maxx = -1;
	int maxy = -1;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int x, y;
		sscanf(line.c_str(), "%d, %d", &x, &y);

		if (minx < 0 || x < minx) minx = x;
		if (miny < 0 || y < miny) miny = y;
		if (x > maxx) maxx = x;
		if (y > maxy) maxy = y;

		nodes.emplace_back(x, y);
	}

	size_t regionsize = 0;
	for (int x = minx; x <= maxx; x++)
	{
		for (int y = miny; y <= maxy; y++)
		{
			int sum = 0;
			for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
			{
				int dx = std::abs(x - iter->first);
				int dy = std::abs(y - iter->second);
				int dis = dx + dy;
				sum += dis;
			}

			if (sum < 10000)
			{
				regionsize++;
			}
		}
	}

	std::cout << regionsize << std::endl;
}
