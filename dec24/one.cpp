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


#define MAXDAMAGETYPECOUNT 8

struct Group
{
	int units;
	int hitpoints;
	int damage;
	int initiative;
	uint8_t damagetype;
	std::bitset<MAXDAMAGETYPECOUNT> weaknesses;
	std::bitset<MAXDAMAGETYPECOUNT> immunities;
	bool evil;

	constexpr int power() const
	{
		return units * damage;
	}

	int calculateDamageTaken(const Group& dealer) const
	{
		if (immunities[dealer.damagetype])
		{
			return 0;
		}
		else if (weaknesses[dealer.damagetype])
		{
			return 2 * dealer.power();
		}
		else return dealer.power();
	}
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

void print(const std::vector<Group>& groups)
{
	//for (const Group& group : groups)
	//{
	//	if (group.evil) std::cout << "Evil group";
	//	else std::cout << "Good group";
	//	std::cout << " containing " << group.units << " units" << std::endl;
	//}
	//std::cout << std::endl;
}

struct Attack
{
	int initiative;
	int attacker;
	int target;

	Attack(int i, int a, int t) :
		initiative(i), attacker(a), target(t)
	{}
};

int main(int /*argc*/, char* /*argv*/[])
{
	std::vector<std::string> damagetypenames;
	std::vector<Group> groups;

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

			groups.emplace_back();
			Group& group = groups.back();

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

					if (parenpos != std::string::npos && line[parenpos] == ';')
					{
						sscanf(line.c_str() + parenpos + 2,
							"weak to %[^;)]",
							buffer);
						parseTypes(damagetypenames, buffer, group.weaknesses);
					}
				}
				else
				{
					sscanf(line.c_str() + parenpos + 1,
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

			group.evil = !parsingImmuneSystem;
		}
	}

	//for (const Group& group : groups)
	//{
	//	if (group.evil) std::cout << "Evil group";
	//	else std::cout << "Good group";
	//	std::cout << " containing " << group.units << " units"
	//		<< " dealing " << group.damage
	//		<< " type " << int(group.damagetype) << " "
	//		<< damagetypenames[group.damagetype] << " damage"
	//		<< " that are immune to " << group.immunities
	//		<< " and weak to " << group.weaknesses
	//		<< std::endl;
	//}
	//std::cout << std::endl;
	//std::cout << std::endl;

	while (true)
	{
		// Sort the groups in descending order.
		std::sort(groups.begin(), groups.end(),
			[](const Group& a, const Group& b) {
				return (a.power() > b.power()
					|| (a.power() == b.power() && a.initiative > b.initiative));
		});

		print(groups);

		std::vector<Attack> attacks;
		for (size_t a = 0; a < groups.size(); a++)
		{
			const Group& attacker = groups[a];
			int besttarget = -1;
			int bestdmg = -1;
			for (size_t i = 0; i < groups.size(); i++)
			{
				const Group& other = groups[i];

				if (other.evil == attacker.evil) continue;

				bool found = false;
				for (const Attack& attack : attacks)
				{
					if (attack.target == i)
					{
						found = true;
						break;
					}
				}
				if (found) continue;

				int dmg = other.calculateDamageTaken(attacker);

				//if (attacker.evil) std::cout << "Evil group";
				//else std::cout << "Good group";
				//std::cout << " " << a << " would deal " << dmg << " damage"
				//	<< " to group " << i << std::endl;

				// Note that groups is already sorted by power and initiative.
				if (dmg > bestdmg)
				{
					besttarget = i;
					bestdmg = dmg;
				}
			}
			attacks.emplace_back(attacker.initiative, a, besttarget);
		}

		// Sort attacks in descending initiative order.
		std::sort(attacks.begin(), attacks.end(),
			[](const Attack& a, const Attack& b) {
				return (a.initiative > b.initiative);
		});

		for (const Attack& attack : attacks)
		{
			if (attack.target < 0) continue;

			const Group& attacker = groups[attack.attacker];
			Group& target = groups[attack.target];

			if (attacker.units <= 0) continue;

			int dmg = target.calculateDamageTaken(attacker);

			//if (attacker.evil) std::cout << "Evil group";
			//else std::cout << "Good group";
			//std::cout << " " << attack.attacker << " deals " << dmg << " damage"
			//	<< " to group " << attack.target
			//	<< " killing " << int(dmg / target.hitpoints) << " units"
			//	<< std::endl;

			target.units -= (dmg / target.hitpoints);
		}

		print(groups);

		groups.erase(
			std::remove_if(groups.begin(), groups.end(),
				[](const Group& a) {
					return (a.units <= 0);
			}),
			groups.end());

		bool immunesystemalive = false;
		bool infectionalive = false;
		for (const Group& group : groups)
		{
			if (group.evil) infectionalive = true;
			else immunesystemalive = true;
		}
		if (!immunesystemalive || !infectionalive) break;
	}

	int totalunits = 0;
	for (const Group& group : groups)
	{
		totalunits += group.units;
	}
	std::cout << "Total units left: " << totalunits << std::endl;
}

// 21164 is too high.
