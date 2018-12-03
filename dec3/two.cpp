//usr/bin/g++ dec3/two.cpp -o dec3/.two; ./dec3/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>


int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec3/input.txt");

	static constexpr int W = 1000;
	static constexpr int H = 1000;
	std::bitset<W * H> claimed;
	std::bitset<W * H> overlapped;
	std::vector<std::bitset<W * H>> claims;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int id, x, y, w, h;
		sscanf(line.c_str(), "#%i @ %i,%i: %ix%i", &id, &x, &y, &w, &h);

		claims.emplace_back();
		for (int r = y; r < y + h && r < H; r++)
		{
			for (int c = x; c < x + w && c < W; c++)
			{
				claims.back()[size_t(r * W + c)] = true;
			}
		}
		overlapped |= (claimed & claims.back());
		claimed |= claims.back();
	}

	for (size_t i = 0; i < claims.size(); i++)
	{
		if ((overlapped & claims[i]).none())
		{
			std::cout << "#" << (i + 1) << " has no overlap." << std::endl;
		}
	}

	std::cout << "Overlap count: " << overlapped.count() << std::endl;
}
