//usr/bin/g++ -g -O2 dec22/two.cpp -o dec22/.two && time ./dec22/.two; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


enum class Tool
{
	NONE, GEAR, TORCH,
};

bool traversible(char type, const Tool& tool)
{
	switch (tool)
	{
		case Tool::TORCH: return (type == '.' || type == '|');
		case Tool::GEAR:  return (type == '.' || type == '=');
		case Tool::NONE:  return (type == '=' || type == '|');
	}
}

Tool change(char fromtype, char totype)
{
	// Pre-condition: fromtype was traversible with old tool but totype is not,
	// hence fromtype != totype. N.B. if x < y < z then x + y < x + z < y + z.
	if      (fromtype + totype == '.' + '|')   return Tool::TORCH;
	else if (fromtype + totype == '.' + '=')   return Tool::GEAR;
	else  /*(fromtype + totype == '=' + '|')*/ return Tool::NONE;
}

struct Position
{
	int x;
	int y;
};

struct Probe
{
	int x;
	int y;
	Tool tool = Tool::TORCH;

	constexpr bool operator==(const Probe& other)
	{
		return (x == other.x && y == other.y && tool == other.tool);
	}

	constexpr bool operator<(const Probe& other)
	{
		return (y < other.y || (y == other.y && x < other.x)
			|| (y == other.y && x == other.x && int(tool) < int(other.tool)));
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	int depth;
	int targetx;
	int targety;
	{
		std::ifstream file("dec22/input.txt");
		std::string line;
		std::getline(file, line);
		sscanf(line.c_str(), "depth: %d", &depth);
		std::getline(file, line);
		sscanf(line.c_str(), "target: %d,%d", &targetx, &targety);
	}

	int width = targetx + targety + 7;
	int height = targety + targetx + 7;

	static int N = 20183;
	std::vector<std::vector<int>> ero(height);
	for (int y = 0; y < height; y++)
	{
		ero[y].resize(width);
		for (int x = 0; x < width; x++)
		{
			if (y == 0)
			{
				if (x == 0)
				{
					ero[y][x] = (depth) % N;
				}
				else
				{
					ero[y][x] = (depth + x * 16807) % N;
				}
			}
			else
			{
				if (x == 0)
				{
					ero[y][x] = (depth + y * 48271) % N;
				}
				else
				{
					ero[y][x] = (depth + ero[y-1][x] * ero[y][x-1]) % N;
				}
			}
		}
	}
	ero[targety][targetx] = (depth) % N;

	std::vector<std::vector<char>> board(height);
	for (int y = 0; y < height; y++)
	{
		board[y].resize(width);
		for (int x = 0; x < width; x++)
		{
			switch (ero[y][x] % 3)
			{
				case 0: board[y][x] = '.'; break;
				case 1: board[y][x] = '='; break;
				case 2: board[y][x] = '|'; break;
			}
		}
	}

	//for (int y = 0; y < height; y++)
	//{
	//	for (int x = 0; x < width; x++)
	//	{
	//		if (x == targetx && y == targety)
	//		{
	//			std::cout << "X";
	//			continue;
	//		}
	//		std::cout << board[y][x];
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::endl;

	std::vector<std::vector<std::bitset<3>>> visited(height,
		std::vector<std::bitset<3>>(width));

	std::array<std::vector<Probe>, 8> queues;
	queues[0] = { {0, 0, Tool::TORCH} };

	bool rescued = false;
	int time = 0;
	while (!rescued && !queues[time % 8].empty())
	{
		auto& queue = queues[time % 8];

		assert(!queue.empty());
		std::sort(queue.begin(), queue.end());
		queue.erase(std::unique(queue.begin(), queue.end()),
			queue.end());

		//std::cout << "t = " << time << std::endl;
		//for (int y = 0; y < height; y++)
		//{
		//	for (int x = 0; x < width; x++)
		//	{
		//		char c = board[y][x];
		//		if (x == targetx && y == targety)
		//		{
		//			c = 'X';
		//		}
		//		for (int t = 1; t <= 7; t++)
		//		{
		//			for (const Probe& probe : queues[(time + t) % 8])
		//			{
		//				if (x == probe.x && y == probe.y)
		//				{
		//					c = 'S';
		//				}
		//			}
		//		}
		//		for (const Probe& probe : queue)
		//		{
		//			if (x == probe.x && y == probe.y)
		//			{
		//				switch (probe.tool)
		//				{
		//					case Tool::TORCH: c = 'T'; break;
		//					case Tool::GEAR:  c = 'G'; break;
		//					case Tool::NONE:  c = 'N'; break;
		//				}
		//			}
		//		}
		//		std::cout << c;
		//	}
		//	std::cout << std::endl;
		//}
		//std::cout << std::endl;

		for (const Probe& probe : queue)
		{
			int x = probe.x;
			int y = probe.y;

			if (x == targetx && y == targety)
			{
				if (probe.tool == Tool::TORCH)
				{
					rescued = true;
					break;
				}
				else
				{
					queues[(time + 7) % 8].push_back(
						{x, y, Tool::TORCH});
					continue;
				}
			}

			visited[y][x][int(probe.tool)] = true;

			for (const Position& pos : {
					Position{x + 1, y}, Position{x, y + 1},
					Position{x - 1, y}, Position{x, y - 1},
				})
			{
				int xx = pos.x;
				int yy = pos.y;

				if (xx < 0) continue;
				if (yy < 0) continue;
				assert(xx < width);
				assert(yy < height);

				// There is no need to change equipment unnecessarily, since
				// change-then-move takes just as much time as move-then-change.
				// Also we can only discard probes if they reach a space that
				// was already visited by a probe with the same tool.
				if (traversible(board[yy][xx], probe.tool)
					&& !visited[yy][xx][int(probe.tool)])
				{
					queues[(time + 1) % 8].push_back(
						{xx, yy, probe.tool});
				}
				else
				{
					Tool newtool = change(board[y][x], board[yy][xx]);
					if (!visited[y][x][int(newtool)])
					{
						queues[(time + 7) % 8].push_back(
							{x, y, newtool});
					}
				}
			}
		}

		if (rescued) break;

		queue.clear();
		for (int t = 1; t <= 7; t++)
		{
			if (!queues[(time + t) % 8].empty())
			{
				time += t;
				break;
			}
		}
	}

	std::cout << "Time: " << time << std::endl;
}
