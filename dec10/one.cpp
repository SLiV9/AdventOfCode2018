//usr/bin/g++ -g -O2 dec10/one.cpp -o dec10/.one && time ./dec10/.one; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>


struct Flare
{
	int xenon;
	int yahoo;
	int hspd;
	int vspd;
};

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec10/input.txt");

	std::vector<Flare> flares;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		int x, y, h, v;
		sscanf(line.c_str(), "position=<%d, %d> velocity=<%d, %d>",
			&x, &y, &h, &v);

		flares.push_back({x, y, h, v});
	}

	for (int t = 0; t < 100000; t++)
	{
		int minx = +10000;
		int miny = +10000;
		int maxx = -10000;
		int maxy = -10000;

		for (Flare& flare : flares)
		{
			if (flare.xenon < minx) minx = flare.xenon;
			if (flare.yahoo < miny) miny = flare.yahoo;
			if (flare.xenon > maxx) maxx = flare.xenon;
			if (flare.yahoo > maxy) maxy = flare.yahoo;
		}

		int w = maxx - minx + 1;
		int h = maxy - miny + 1;

		//if (t > 9900 && t < 10100)
		if (t > 10000 && t < 10020)
		{
			std::cout << "t = " << t << ": " << w << "x" << h << std::endl;
		}

		if (t == 10009)
		{
			std::cout << std::endl;
			std::vector<bool> paper(w * h, false);
			for (Flare& flare : flares)
			{
				int c = flare.xenon - minx;
				int r = flare.yahoo - miny;
				paper[r * w + c] = true;
			}
			for (int r = 0; r < h; r++)
			{
				for (int c = 0; c < w; c++)
				{
					if (paper[r * w + c]) std::cout << "#";
					else std::cout << ".";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		for (Flare& flare : flares)
		{
			flare.xenon += flare.hspd;
			flare.yahoo += flare.vspd;
		}
	}
}

// Although it would be nice to visualize this in a gif of some sort, I decided
// to go straight for the quickest solution and look for the timestamp where
// the flares converged, i.e. where the circumscribing rectangle was smallest.
