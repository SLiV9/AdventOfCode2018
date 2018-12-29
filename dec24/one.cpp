//usr/bin/g++ -g -O2 dec24/one.cpp -o dec24/.one && time ./dec24/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


#define MAXDAMAGETYPECOUNT 12

struct Group
{
	int units;
	int hitpoints;
	int damage;
	int initiative;
	// 8 + 12 + 12 = 32
	uint8_t damagetype;
	std::bitset<MAXDAMAGETYPECOUNT> weaknesses;
	std::bitset<MAXDAMAGETYPECOUNT> immunities;
};

uint8_t parseType(std::vector<std::string>& names, const char* buffer)
{
	std::string damagetype = buffer;
	uint8_t d = 0;
	for (; d < names.size(); d++)
	{
		if (names[d] == damagetype)
		{
			break;
		}
	}
	if (d >= names.size())
	{
		assert(d < MAXDAMAGETYPECOUNT);
		names.emplace_back(damagetype);
	}
	return d;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::vector<std::string> damagetypenames;

	std::vector<Group> immunesystem;
	std::vector<Group> infection;

	{
		std::ifstream file("dec24/test.txt");
		std::string line;
		bool parsingImmuneSystem = true;
		while (std::getline(file, line))
		{
			if (line.empty()) continue;

			if (line.find(':') != std::string::npos)
			{
				parsingImmuneSystem = (line == "Immune System:");
				continue;
			}

			char immunitiesbuffer[80];
			char weaknessesbuffer[80];
			char damagetypebuffer[80];

			Group group;
			int hits = sscanf(line.c_str(),
				"%d units each with %d hit points"
				" (immune to %[^;]; weak to %[^)])"
				" with an attack that does %d %s damage"
				" at initiative %d",
				&(group.units), &(group.hitpoints),
				immunitiesbuffer, weaknessesbuffer,
				&(group.damage), damagetypebuffer,
				&(group.initiative));
			std::cout << "'" << line << "' ==> " << hits << std::endl;
			assert(hits == 7);

			{
				std::stringstream strm(immunitiesbuffer);
				std::string token;
				while (std::getline(strm, token, ','))
				{
					const char* tokenc = token.c_str();
					if (*tokenc == ' ') tokenc++;

					uint8_t d = parseType(damagetypenames, tokenc);
					group.immunities[d] = true;
				}
			}

			{
				std::stringstream strm(weaknessesbuffer);
				std::string token;
				while (std::getline(strm, token, ','))
				{
					const char* tokenc = token.c_str();
					if (*tokenc == ' ') tokenc++;

					uint8_t d = parseType(damagetypenames, tokenc);
					group.weaknesses[d] = true;
				}
			}

			group.damagetype = parseType(damagetypenames, damagetypebuffer);
		}
	}

}
