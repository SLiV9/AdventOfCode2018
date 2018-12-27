//usr/bin/g++ -g -O2 dec23/two.cpp -o dec23/.two && time ./dec23/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


constexpr int limit(int x, int min, int max)
{
	return std::min(std::max(min, x), max);
}

struct Point;
struct Bot;

int inrange(const Point& point, const Bot& bot);

struct Point
{
	int x;
	int y;
	int z;

	Point() :
		Point(0, 0, 0)
	{}

	Point(int xx, int yy, int zz) :
		x(xx), y(yy), z(zz)
	{}

	int distanceTo(const Point& other) const
	{
		return (std::abs(x - other.x) + std::abs(y - other.y)
			+ std::abs(z - other.z));
	}

	int inrange(const std::vector<Bot>& bots) const
	{
		int ir = 0;
		for (const Bot& bot : bots)
		{
			if (::inrange(*this, bot)) ir++;
		}
		return ir;
	}
};

std::ostream& operator<<(std::ostream& os, const Point& point)
{
	os << point.x << "," << point.y << "," << point.z;
}

struct Bot : public Point
{
	int r;

	Bot(int xx, int yy, int zz, int rr) :
		Point(xx, yy, zz),
		r(rr)
	{}
};

int inrange(const Point& point, const Bot& bot)
{
	return bot.distanceTo(point) <= bot.r;
}

std::ostream& operator<<(std::ostream& os, const Bot& bot)
{
	os << ((const Point&) bot) << ":" << bot.r;
}

struct Box
{
	Point min;
	Point max;

	Box()
	{}

	int volume() const
	{
		return (max.x - min.x + 1) * (max.y - min.y + 1) * (max.z - min.z + 1);
	}

	int inrange(const std::vector<Bot>& bots) const
	{
		int ir = 0;
		for (const Bot& bot : bots)
		{
			if (shadow(bot).distanceTo(bot) <= bot.r) ir++;
		}
		return ir;
	}

	Point shadow(Point it) const
	{
		return Point(limit(it.x, min.x, max.x), limit(it.y, min.y, max.y),
			limit(it.z, min.z, max.z));
	}
};

std::ostream& operator<<(std::ostream& os, const Box& box)
{
	os << "[" << box.min << "; " << box.max << "]";
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec23/input.txt");

	std::vector<Bot> bots;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int x, y, z, r;
		sscanf(line.c_str(), "pos=<%d,%d,%d>, r=%d",
			&x, &y, &z, &r);
		bots.push_back({x, y, z, r});
	}

	Box bbox;
	bbox.min.x = bots[0].x;
	bbox.min.y = bots[0].y;
	bbox.min.z = bots[0].z;
	bbox.max.x = bots[0].x;
	bbox.max.y = bots[0].y;
	bbox.max.z = bots[0].z;

	int bestrange = -1;
	size_t bestindex = 0;

	for (size_t i = 0; i < bots.size(); i++)
	{
		if (bots[i].r > bestrange)
		{
			bestrange = bots[i].r;
			bestindex = i;
		}

		if (bots[i].x < bbox.min.x) bbox.min.x = bots[i].x;
		if (bots[i].y < bbox.min.y) bbox.min.y = bots[i].y;
		if (bots[i].z < bbox.min.z) bbox.min.z = bots[i].z;
		if (bots[i].x > bbox.max.x) bbox.max.x = bots[i].x;
		if (bots[i].y > bbox.max.y) bbox.max.y = bots[i].y;
		if (bots[i].z > bbox.max.z) bbox.max.z = bots[i].z;
	}

	std::cout << "Strongest bot is bots[" << bestindex << "]"
		" with range " << bestrange << std::endl;

	// I think it is provably impossible for the best position to be outside
	// of the bounding box that surrounds all of the bots. For any point outside
	// that box, the closest border point will always be in range of more bots.
	std::cout << bbox << std::endl;

	constexpr int divisor = 2;

	int bestscore = -1;
	int bestdis = -1;
	Point bestpoint;

	for (const Bot& bot : bots)
	{
		int score = bot.inrange(bots);
		int dis = bot.distanceTo(Point(0, 0, 0));
		if (score > bestscore
			|| (score == bestscore
				&& (bestdis < 0 || dis < bestdis)))
		{
			bestscore = score;
			bestdis = dis;
			bestpoint = bot;
			//std::cout << "Best so far: " << bestpoint << " with "
			//	<< bestscore << " at " << bestdis << std::endl;
		}
	}

	std::vector<Box> queue;
	std::vector<Box> nextqueue;
	queue.push_back(bbox);
	while (!queue.empty())
	{
		for (const Box& box : queue)
		{
			int w = box.max.x - box.min.x + 1;
			int h = box.max.y - box.min.y + 1;
			int d = box.max.z - box.min.z + 1;

			if (box.volume() > 1)
			{
				Point center;
				center.x = box.min.x + w / 2;
				center.y = box.min.y + h / 2;
				center.z = box.min.z + d / 2;
				int score = center.inrange(bots);
				int dis = center.distanceTo(Point(0, 0, 0));
				if (score > bestscore
					|| (score == bestscore
						&& (bestdis < 0 || dis < bestdis)))
				{
					bestscore = score;
					bestdis = dis;
					bestpoint = center;
					//std::cout << "Best so far: " << bestpoint
					//	<< " with " << bestscore
					//	<< " at " << bestdis << std::endl;
				}
			}

			Box sub;
			for (int k = 0; k < divisor; k++)
			{
				sub.min.z = box.min.z + d *  k      / divisor;
				sub.max.z = box.min.z + d * (k + 1) / divisor - 1;
				if (sub.max.z < sub.min.z) continue;

				for (int j = 0; j < divisor; j++)
				{
					sub.min.y = box.min.y + h *  j      / divisor;
					sub.max.y = box.min.y + h * (j + 1) / divisor - 1;
					if (sub.max.y < sub.min.y) continue;

					for (int i = 0; i < divisor; i++)
					{
						sub.min.x = box.min.x + w *  i      / divisor;
						sub.max.x = box.min.x + w * (i + 1) / divisor - 1;
						if (sub.max.x < sub.min.x) continue;

						if (sub.volume() == 1)
						{
							int score = sub.min.inrange(bots);
							int dis = sub.min.distanceTo(Point(0, 0, 0));
							if (score > bestscore
								|| (score == bestscore
									&& (bestdis < 0 || dis < bestdis)))
							{
								bestscore = score;
								bestdis = dis;
								bestpoint = sub.min;
								//std::cout << "Best so far: " << bestpoint
								//	<< " with " << bestscore
								//	<< " at " << bestdis << std::endl;
							}
						}
						else
						{
							int score = sub.inrange(bots);
							int dis = sub.min.distanceTo(Point(0, 0, 0));
							if (score > bestscore
								|| (score == bestscore
									&& (bestdis < 0 || dis < bestdis)))
							{
								//std::cout << sub << "\t" << score << std::endl;
								nextqueue.push_back(sub);
							}
						}
					}
				}
			}
		}

		queue.clear();
		if (!nextqueue.empty())
		{
			nextqueue.swap(queue);
		}
	}

	std::cout << "Best score: " << bestscore << std::endl;
	std::cout << "Best point: " << bestpoint
		<< " (distance to origin: " << bestdis << ")" << std::endl;
}


//     #
//    ###      #
//   #####    ###    #
//  ###3###  ##2##  #1#  0
//   #####    ###    #
//    ###      #
//     #
