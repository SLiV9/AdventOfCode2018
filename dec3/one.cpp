//usr/bin/g++ dec3/one.cpp -o dec3/.one; ./dec3/.one; exit

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

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int id, x, y, w, h;
		sscanf(line.c_str(), "#%i @ %i,%i: %ix%i", &id, &x, &y, &w, &h);

		for (int r = y; r < y + h && r < H; r++)
		{
			for (int c = x; c < x + w && c < W; c++)
			{
				const size_t i = size_t(r * W + c);
				if (claimed[i]) overlapped[i] = true;
				claimed[i] = true;
			}
		}
	}

	std::cout << "Overlap count: " << overlapped.count() << std::endl;
}
