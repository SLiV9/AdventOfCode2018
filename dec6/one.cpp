//usr/bin/g++ -O2 dec6/one.cpp -o dec6/.one && time ./dec6/.one; exit

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

	// Overthinking it again.
	//std::sort(nodes.begin(), nodes.end());

	std::vector<std::vector<std::pair<size_t, uint8_t>>> grid;
	for (int x = minx; x <= maxx; x++)
	{
		grid.emplace_back();
		auto& row = grid.back();

		//auto firstnode = std::lower_bound(nodes.begin(), nodes.end(),
		//	std::pair<int, int>(x, 0));
		auto firstnode = nodes.begin();

		for (int y = miny; y <= maxy; y++)
		{
			size_t shortest = size_t(-1);
			uint8_t color = WHITE;

			//if (x > minx)
			//{
			//	shortest = 1 + grid[(x - 1) - minx][y - miny].first;
			//	color = grid[(x - 1) - minx][y - miny].second;
			//}

			//if (y > miny)
			//{
			//	int dis = 1 + row.back().first;
			//	if (dis < shortest)
			//	{
			//		shortest = dis;
			//		color = row.back().second;
			//	}
			//	else if (dis == shortest)
			//	{
			//		color = paint(color, row.back().second);
			//	}
			//}

			for (auto iter = firstnode; iter != nodes.end(); ++iter)
			{
				int dx = std::abs(x - iter->first);
				int dy = std::abs(y - iter->second);
				int dis = dx + dy;
				if (dis < shortest)
				{
					shortest = dis;
					color = uint8_t(iter - nodes.begin());
				}
				else if (dis == shortest)
				{
					color = paint(color, uint8_t(iter - nodes.begin()));
				}
			}

			row.emplace_back(shortest, color);
		}
	}

	std::vector<bool> border(nodes.size(), false);
	std::vector<int> count(nodes.size(), 0);
	for (int x = minx; x <= maxx; x++)
	{
		for (int y = miny; y <= maxy; y++)
		{
			uint8_t color = grid[x - minx][y - miny].second;
			if (color != WHITE && color != BLACK && !border[color])
			{
				if (x == minx || x == maxx || y == miny || y == maxy)
				{
					border[color] = true;
				}
				else count[color]++;
			}
		}
	}

	//for (int y = miny; y <= maxy; y++)
	//{
	//	for (int x = minx; x <= maxx; x++)
	//	{
	//		uint8_t color = grid[x - minx][y - miny].second;
	//		if (color == WHITE)
	//		{
	//			std::cout << '?';
	//		}
	//		else if (color == BLACK)
	//		{
	//			std::cout << ".";
	//		}
	//		else if (grid[x - minx][y - miny].first == 0)
	//		{
	//			std::cout << char('A' + color);
	//		}
	//		else
	//		{
	//			std::cout << char('a' + color);
	//		}
	//	}
	//	std::cout << std::endl;
	//}

	{
		auto iter = std::max_element(count.begin(), count.end());
		std::cout << std::to_string(*iter) << std::endl;
	}
}

// A region is infinite iff it dominates a point on the border, because all
// points perpendicular to the border starting from that point will also be
// dominated by that region. If is tied, all perpendicular points will also be
// tied. (Because the competing node cannot be outside the border).

//    (c)
// #a##c####
// #AaacbbB#
// #a-dccbbb
// #-dDCcc-#
// #e-dc-ff#
// eEe--ffF#
// ###-###f#
//   (-)
