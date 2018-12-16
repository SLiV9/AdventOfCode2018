//usr/bin/g++ -g -O2 dec16/two.cpp -o dec16/.two && time ./dec16/.two; exit

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

	std::bitset<size_t(Operation::SIZE)> couldbe[16];

	State state = State::SAMPLEBEFORE;
	std::string line;
	while (state != State::PROGRAM && std::getline(file, line))
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

				//std::cout << line << std::endl;
				scanRegister(line, reg);

				state = State::SAMPLEINSTRUCTION;
			}
			break;
			case State::SAMPLEINSTRUCTION:
			{
				//std::cout << line << std::endl;
				scanInstruction(line, opcode, argA, argB, argC);

				state = State::SAMPLEAFTER;
			}
			break;
			case State::SAMPLEAFTER:
			{
				//std::cout << line << std::endl;
				Register out;
				scanRegister(line, out);

				std::bitset<size_t(Operation::SIZE)> matches;

				//std::cout << "Matches ";
				//const char* sep = "";
				for (uint8_t i = 0; i < uint8_t(Operation::SIZE); i++)
				{
					if (out[argC] == operate((Operation) i, argA, argB, reg))
					{
						//std::cout << sep << (Operation) i;
						//sep = ", ";

						matches[i] = true;
					}
				}
				//if (matches.none()) std::cout << "nothing" << std::endl;
				//else std::cout << " (" << matches.count() << ")" << std::endl;

				if (couldbe[opcode].any())
				{
					matches = couldbe[opcode] &= matches;
				}
				else couldbe[opcode] = matches;

				//std::cout << "Opcode " << int(opcode) << " could be ";
				//sep = "";
				//for (uint8_t i = 0; i < uint8_t(Operation::SIZE); i++)
				//{
				//	if (matches[i])
				//	{
				//		std::cout << sep << (Operation) i;
				//		sep = ", ";
				//	}
				//}
				//if (matches.none()) std::cout << "nothing" << std::endl;
				//else std::cout << " (" << matches.count() << ")" << std::endl;
				//std::cout << std::endl;

				state = State::SAMPLESEPARATOR;
			}
			break;
			case State::SAMPLESEPARATOR:
			{
				state = State::SAMPLEBEFORE;
			}
			break;
		}
	}

	for (opcode = 0; opcode < 16; opcode++)
	{
		std::cout << "Opcode " << int(opcode) << " could be ";
		const char* sep = "";
		for (uint8_t i = 0; i < uint8_t(Operation::SIZE); i++)
		{
			if (couldbe[opcode][i])
			{
				std::cout << sep << (Operation) i;
				sep = ", ";
			}
		}
		if (couldbe[opcode].none()) std::cout << "nothing" << std::endl;
		else std::cout << " (" << couldbe[opcode].count() << ")" << std::endl;
	}
	std::cout << std::endl;

	Operation operation[16];

	std::bitset<16> solved;
	while (solved.count() < 16)
	{
		for (opcode = 0; opcode < 16; opcode++)
		{
			assert(couldbe[opcode].any());
			if (solved[opcode]) continue;
			if (couldbe[opcode].count() > 1) continue;

			solved[opcode] = true;

			for (uint8_t i = 0; i < uint8_t(Operation::SIZE); i++)
			{
				if (couldbe[opcode][i])
				{
					operation[opcode] = (Operation) i;
				}
			}

			for (uint8_t o = 0; o < 16; o++)
			{
				if (solved[o]) continue;
				couldbe[o] &= ~(couldbe[opcode]);
			}

			std::cout << "Opcode " << int(opcode) << " is "
				<< operation[opcode] << std::endl;
		}
	}
	std::cout << std::endl;

	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 0;

	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		scanInstruction(line, opcode, argA, argB, argC);

		std::cout << operation[opcode] << " " << int(argA) << " " << int(argB)
			<< " " << int(argC) << std::endl;

		reg[argC] = operate(operation[opcode], argA, argB, reg);

		std::cout << "[" << int(reg[0]) << ", " << int(reg[1]) << ", "
			<< int(reg[2]) << ", " << int(reg[3]) << "]" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "The answer is " << int(reg[0]) << std::endl;
}
