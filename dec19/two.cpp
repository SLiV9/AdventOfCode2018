//usr/bin/g++ -g -O2 dec19/two.cpp -o dec19/.two && time ./dec19/.two; exit

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

std::ostream& operator<<(std::ostream& os, int reg[6])
{
	const char* sep = "[";
	for (int i = 0; i < 6; i++)
	{
		os << sep << reg[i];
		sep = ", ";
	}
	return os << "]";
}

int operate(const Operation& op, int argA, int argB, int reg[6])
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
	int argA;
	int argB;
	int argC;

	void operate(int reg[6]) const
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
	return {parseOperation(o), a, b, c};
}

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec19/input.txt");

	int reg[6] = {0, 0, 0, 0, 0, 0};
	std::vector<Instruction> program;
	int icpos;

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

	reg[0] = 1;

	size_t programsize = program.size();
	for (size_t ic = 0; ic < programsize; ic++)
	{
		if (ic == 3)
		{
			std::cout << "Skip." << std::endl;
			for (int i = 1; i <= reg[2]; i++)
			{
				if (reg[2] % i == 0)
				{
					reg[0] += i;
				}
			}
			break;
		}

		reg[icpos] = ic;

		std::cout << "ip=" << ic << " " << reg << " " << program[ic];

		program[ic].operate(reg);

		std::cout << " " << reg << std::endl;

		ic = reg[icpos];
	}

	std::cout << "The answer is " << reg[0] << std::endl;
}

// After initializing, #1 = 1 and #2 = N. Now #0 is the sum of the divisors of #n.

// #ip 4
// addi 4 16 4   //  0: jump to 16+1
// seti 1 8 1    //  1: finishing up initializing
// seti 1 3 5    // (loop goes from 3 to 11 inclusive)
// mulr 1 5 3    //  3: #3 = #1 * #5                     // do { do {               // do {                     // for (; #1 <= #2; #1++) {
// eqrr 3 2 3    //   : #3 = (#3 == #2)                  //   if (#1 * #5 == #2)    //   if (#2 % #1 == 0) {    //   if (#2 % #1 == 0) {
// addr 3 4 4    //   : ic += #3                         //   {
// addi 4 1 4    //   : ic += 1                          //     #0 += #1;
// addr 1 0 0    //   : #0 = #0 + #1                     //   }                     //     #0 += #1;            //     #0 += #1;
// addi 5 1 5    //   : #5 += 1                          //   #5++;
// gtrr 5 2 3    //   : #3 = (#5 > #2)                   //
// addr 4 3 4    //   : ic += #3                         //
// seti 2 2 4    // 11: loop back to 2+1                 // } while (#5 <= #2)      //   }                      //   }
// addi 1 1 1    //   : #1 += 1                          //   #1++;                 //   #1++;
// gtrr 1 2 3    //   : #3 = (#1 > #2)                   // } while (#1 <= #2)      // } while (#1 <= #2)       // }
// addr 3 4 4    //   : ic += #3
// seti 1 4 4    //   : jump to 1+1
// mulr 4 4 4    // 16: ic = ic * ic                     // exit
// addi 2 2 2    // 17: start initializing
// mulr 2 2 2
// mulr 4 2 2
// muli 2 11 2
// addi 3 6 3
// mulr 3 4 3
// addi 3 8 3
// addr 2 3 2
// addr 4 0 4
// seti 0 1 4
// setr 4 4 3
// mulr 3 4 3
// addr 4 3 3
// mulr 4 3 3
// muli 3 14 3
// mulr 3 4 3
// addr 2 3 2
// seti 0 4 0
// seti 0 7 4    // 35: jump to 0+1

