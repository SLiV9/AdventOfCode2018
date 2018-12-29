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

void parseTypes(std::vector<std::string>& names, const char* buffer,
	std::bitset<MAXDAMAGETYPECOUNT>& types)
{
	std::stringstream strm(buffer);
	std::string token;
	while (std::getline(strm, token, ','))
	{
		const char* tokenc = token.c_str();
		if (*tokenc == ' ') tokenc++;

		uint8_t d = parseType(names, tokenc);
		types[d] = true;
	}
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::vector<std::string> damagetypenames;

	std::vector<Group> immunesystem;
	std::vector<Group> infection;

	{
		std::ifstream file("dec24/input.txt");
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

			Group group;
			int hits = sscanf(line.c_str(),
				"%d units each with %d hit points",
				&(group.units), &(group.hitpoints));
			assert(hits == 2);

			char buffer[80];

			size_t parenpos = line.find('(');
			if (parenpos != std::string::npos)
			{
				if (line[parenpos + 1] == 'i')
				{
					sscanf(line.c_str() + parenpos + 1,
						"immune to %[^;)]",
						buffer);
					parseTypes(damagetypenames, buffer, group.immunities);

					parenpos = line.find_first_of(";)", parenpos);
				}

				if (parenpos != std::string::npos
					&& line[parenpos + 2] == 'w')
				{
					sscanf(line.c_str() + parenpos + 2,
						"weak to %[^;)]",
						buffer);
					parseTypes(damagetypenames, buffer, group.weaknesses);
				}

				parenpos = line.find(")", parenpos);
			}
			else
			{
				parenpos = line.find(" with an attack") - 1;
			}

			hits = sscanf(line.c_str() + parenpos + 1,
				" with an attack that does %d %s damage at initiative %d",
				&(group.damage), buffer, &(group.initiative));
			assert(hits == 3);

			group.damagetype = parseType(damagetypenames, buffer);
		}
	}

}
