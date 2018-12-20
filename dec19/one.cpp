//usr/bin/g++ -g -O2 dec19/one.cpp -o dec19/.one && time ./dec19/.one; exit

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <cstring>


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

Operation parseOperation(const char* str)
{
	if      (strncmp(str, "addr", 4) == 0) return Operation::ADDR;
	else if (strncmp(str, "addi", 4) == 0) return Operation::ADDI;
	else if (strncmp(str, "mulr", 4) == 0) return Operation::MULR;
	else if (strncmp(str, "muli", 4) == 0) return Operation::MULI;
	else if (strncmp(str, "banr", 4) == 0) return Operation::BANR;
	else if (strncmp(str, "bani", 4) == 0) return Operation::BANI;
	else if (strncmp(str, "borr", 4) == 0) return Operation::BORR;
	else if (strncmp(str, "bori", 4) == 0) return Operation::BORI;
	else if (strncmp(str, "setr", 4) == 0) return Operation::SETR;
	else if (strncmp(str, "seti", 4) == 0) return Operation::SETI;
	else if (strncmp(str, "gtir", 4) == 0) return Operation::GTIR;
	else if (strncmp(str, "gtri", 4) == 0) return Operation::GTRI;
	else if (strncmp(str, "gtrr", 4) == 0) return Operation::GTRR;
	else if (strncmp(str, "eqir", 4) == 0) return Operation::EQIR;
	else if (strncmp(str, "eqri", 4) == 0) return Operation::EQRI;
	else if (strncmp(str, "eqrr", 4) == 0) return Operation::EQRR;
	else throw std::runtime_error("Unknown '" + std::string(str) + "'");
}

const char* stringify(const Operation& op)
{
	switch (op)
	{
		case Operation::ADDR: return "addr";
		case Operation::ADDI: return "addi";
		case Operation::MULR: return "mulr";
		case Operation::MULI: return "muli";
		case Operation::BANR: return "banr";
		case Operation::BANI: return "bani";
		case Operation::BORR: return "borr";
		case Operation::BORI: return "bori";
		case Operation::SETR: return "setr";
		case Operation::SETI: return "seti";
		case Operation::GTIR: return "gtir";
		case Operation::GTRI: return "gtri";
		case Operation::GTRR: return "gtrr";
		case Operation::EQIR: return "eqir";
		case Operation::EQRI: return "eqri";
		case Operation::EQRR: return "eqrr";
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
	int data[6] = {0, 0, 0, 0, 0, 0};

public:
	int operator[](uint8_t i) const
	{
		return data[i % 6];
	}

	int& operator[](uint8_t i)
	{
		return data[i % 6];
	}
};

std::ostream& operator<<(std::ostream& os, const Register& reg)
{
	const char* sep = "[";
	for (uint8_t i = 0; i < 6; i++)
	{
		os << sep << reg[i];
		sep = ", ";
	}
	return os << "]";
}

int operate(const Operation& op, int argA, int argB, Register& reg)
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

struct Instruction
{
	Operation op;
	uint8_t argA;
	uint8_t argB;
	uint8_t argC;

	void operate(Register& reg) const
	{
		reg[argC] = ::operate(op, argA, argB, reg);
	}
};

std::ostream& operator<<(std::ostream& os, const Instruction& instr)
{
	return os << stringify(instr.op) << " " << int(instr.argA)
		<< " " << int(instr.argB) << " " << int(instr.argC);
}

Instruction parseInstruction(const char* str)
{
	char o[5] = "abcd";
	int a, b, c;
	sscanf(str, "%4s %d %d %d", o, &a, &b, &c);
	return {parseOperation(o), uint8_t(a), uint8_t(b), uint8_t(c)};
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec19/input.txt");

	Register reg;
	std::vector<Instruction> program;
	uint8_t icpos;

	std::string line;
	{
		std::getline(file, line);
		int i;
		sscanf(line.c_str(), "#ip %d", &i);
		icpos = (i % 6);

	}

	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		program.emplace_back(parseInstruction(line.c_str()));
	}

	for (size_t ic = 0; ic < program.size(); ic++)
	{
		reg[icpos] = ic;

		//std::cout << "ip=" << ic << " " << reg << " " << program[ic];

		program[ic].operate(reg);

		//std::cout << " " << reg << std::endl;

		ic = reg[icpos];
	}

	std::cout << "The answer is " << reg[0] << std::endl;
}
