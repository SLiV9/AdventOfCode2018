//usr/bin/g++ -g -O2 dec25/one.cpp -o dec25/.one && time ./dec25/.one "$@"; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


struct Point
{
	int x;
	int y;
	int z;
	int u;

	constexpr int distanceTo(const Point& other) const
	{
		return std::abs(x - other.x) + std::abs(y - other.y)
			+  std::abs(z - other.z) + std::abs(u - other.u);
	}
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	return os << p.x << "," << p.y << "," << p.z << "," << p.u;
}

int main(int argc, char* argv[])
{
	std::vector<Point> points;

	{
		const char* filename = "dec25/input.txt";
		if (argc > 1) filename = argv[1];
		std::ifstream file(filename);
		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty()) continue;

			points.emplace_back();
			Point& point = points.back();

			sscanf(line.c_str(), "%d,%d,%d,%d",
				&(point.x), &(point.y), &(point.z), &(point.u));
		}
	}

	for (const Point& point : points)
	{
		std::cout << point << std::endl;
	}
	std::cout << std::endl;

	std::vector<char> processed(points.size(), false);
	std::vector<std::vector<Point>> constellations;

	for (size_t i = 0; i < points.size(); i++)
	{
		if (processed[i]) continue;

		constellations.emplace_back();
		auto& con = constellations.back();

		con.push_back(points[i]);
		processed[i] = true;

		for (int fresh = 1; fresh > 0; fresh--)
		{
			for (size_t j = i + 1; j < points.size(); j++)
			{
				if (processed[j]) continue;

				bool connected = false;
				for (const Point& star : con)
				{
					if (points[j].distanceTo(star) <= 3)
					{
						connected = true;
						break;
					}
				}

				if (connected)
				{
					con.push_back(points[j]);
					processed[j] = true;
					fresh = 2;
				}
			}
		}
	}

	for (const auto& con : constellations)
	{
		const char* sep = "";
		for (const Point& point : con)
		{
			std::cout << sep << point;
			sep = "; ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Constellations: " << constellations.size() << std::endl;
}

// 21164 is too high.
