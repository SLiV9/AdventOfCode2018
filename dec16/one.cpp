//usr/bin/g++ -g -O2 dec16/one.cpp -o dec16/.one && time ./dec16/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


enum class State
{
	SAMPLEBEFORE,
	SAMPLEINSTRUCTION,
	SAMPLEAFTER,
	SAMPLESEPARATOR,
	PROGRAM,
};

enum class Operation
{
	ADDR, ADDI,
	MULR, MULI,
	BANR, BANI,
	BORR, BORI,
	SETR, SETI,
	GTIR, GTRI, GTRR,
	EQIR, EQRI, EQRR,

	// Metavariable.
	SIZE
};

const char* stringify(const Operation& op)
{
	switch (op)
	{
		case Operation::ADDR: return "ADDR";
		case Operation::ADDI: return "ADDI";
		case Operation::MULR: return "MULR";
		case Operation::MULI: return "MULI";
		case Operation::BANR: return "BANR";
		case Operation::BANI: return "BANI";
		case Operation::BORR: return "BORR";
		case Operation::BORI: return "BORI";
		case Operation::SETR: return "SETR";
		case Operation::SETI: return "SETI";
		case Operation::GTIR: return "GTIR";
		case Operation::GTRI: return "GTRI";
		case Operation::GTRR: return "GTRR";
		case Operation::EQIR: return "EQIR";
		case Operation::EQRI: return "EQRI";
		case Operation::EQRR: return "EQRR";
	}
	return "error";
}

std::ostream& operator<<(std::ostream& os, const Operation& op)
{
	return os << stringify(op);
}

class Register
{
private:
	uint8_t data[4] = {0, 0, 0, 0};

public:
    uint8_t operator[](uint8_t i) const
    {
    	return data[i & 0x3] & 0xF;
    }

    uint8_t& operator[](uint8_t i)
    {
    	return data[i & 0x3];
    }
};

uint8_t operate8(const Operation& op, uint8_t argA, uint8_t argB, Register& reg)
{
	switch (op)
	{
		case Operation::ADDR:
		{
			return reg[argA] + reg[argB];
		}
		break;
		case Operation::ADDI:
		{
			return reg[argA] + argB;
		}
		break;
		case Operation::MULR:
		{
			return reg[argA] * reg[argB];
		}
		break;
		case Operation::MULI:
		{
			return reg[argA] * argB;
		}
		break;
		case Operation::BANR:
		{
			return reg[argA] & reg[argB];
		}
		break;
		case Operation::BANI:
		{
			return reg[argA] & argB;
		}
		break;
		case Operation::BORR:
		{
			return reg[argA] | reg[argB];
		}
		break;
		case Operation::BORI:
		{
			return reg[argA] | argB;
		}
		break;
		case Operation::SETR:
		{
			return reg[argA];
		}
		break;
		case Operation::SETI:
		{
			return argA;
		}
		break;
		case Operation::GTIR:
		{
			return (argA > reg[argB]);
		}
		break;
		case Operation::GTRI:
		{
			return (reg[argA] > argB);
		}
		break;
		case Operation::GTRR:
		{
			return (reg[argA] > reg[argB]);
		}
		break;
		case Operation::EQIR:
		{
			return (argA == reg[argB]);
		}
		break;
		case Operation::EQRI:
		{
			return (reg[argA] == argB);
		}
		break;
		case Operation::EQRR:
		{
			return (reg[argA] == reg[argB]);
		}
		break;
	}
	return 0;
}

uint8_t operate(const Operation& op, uint8_t argA, uint8_t argB, Register& reg)
{
	return operate8(op, argA, argB, reg) & 0xF;
}

void scanRegister(const std::string& line,
	Register& reg)
{
	int in[4];
	sscanf(line.c_str() + strlen("Before: ["), "%d, %d, %d, %d",
		&in[0], &in[1], &in[2], &in[3]);
	for (uint8_t i = 0; i < 4; i++)
	{
		reg[i] = in[i] & 0xF;
	}
}

void scanInstruction(const std::string& line,
	uint8_t& opcode, uint8_t& argA, uint8_t& argB, uint8_t& argC)
{
	int o, a, b, c;
	sscanf(line.c_str(), "%d %d %d %d", &o, &a, &b, &c);
	opcode = o & 0xF;
	argA = a & 0xF;
	argB = b & 0xF;
	argC = c & 0xF;
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec16/input.txt");

	Register reg;
	uint8_t opcode;
	uint8_t argA;
	uint8_t argB;
	uint8_t argC;

	int matches3ormore = 0;

	State state = State::SAMPLEBEFORE;
	std::string line;
	while (std::getline(file, line))
	{
		switch (state)
		{
			case State::SAMPLEBEFORE:
			{
				if (line.empty())
				{
					state = State::PROGRAM;
					break;
				}

				std::cout << line << std::endl;
				scanRegister(line, reg);

				state = State::SAMPLEINSTRUCTION;
			}
			break;
			case State::SAMPLEINSTRUCTION:
			{
				std::cout << line << std::endl;
				scanInstruction(line, opcode, argA, argB, argC);

				state = State::SAMPLEAFTER;
			}
			break;
			case State::SAMPLEAFTER:
			{
				std::cout << line << std::endl;
				Register out;
				scanRegister(line, out);

				int matches = 0;
				std::cout << "Matches ";
				const char* sep = "";
				for (uint8_t i = 0; i < uint8_t(Operation::SIZE); i++)
				{
					if (out[argC] == operate((Operation) i, argA, argB, reg))
					{
						std::cout << sep << (Operation) i;
						sep = ", ";

						matches++;
					}
				}

				if (matches == 0) std::cout << "nothing" << std::endl;
				else std::cout << " (" << matches << ")" << std::endl;
				std::cout << std::endl;

				if (matches >= 3)
				{
					matches3ormore++;
				}

				state = State::SAMPLESEPARATOR;
			}
			break;
			case State::SAMPLESEPARATOR:
			{
				state = State::SAMPLEBEFORE;
			}
			break;
			case State::PROGRAM:
			{
				break;
			}
			break;
		}
	}

	std::cout << matches3ormore << " samples match 3 or more" << std::endl;
}
