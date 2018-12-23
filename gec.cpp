//usr/bin/g++ -g -O2 gec.cpp -o gec; exit

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

int maxIndexUsed(const Operation& op, int argA, int argB)
{
	switch (op)
	{
		case Operation::ADDR: return std::max(argA, argB);
		break;
		case Operation::ADDI: return 0;
		break;
		case Operation::MULR: return std::max(argA, argB);
		break;
		case Operation::MULI: return argA;
		break;
		case Operation::BANR: return std::max(argA, argB);
		break;
		case Operation::BANI: return argA;
		break;
		case Operation::BORR: return std::max(argA, argB);
		break;
		case Operation::BORI: return argA;
		break;
		case Operation::SETR: return argA;
		break;
		case Operation::SETI: return 0;
		break;
		case Operation::GTIR: return argB;
		break;
		case Operation::GTRI: return argA;
		break;
		case Operation::GTRR: return std::max(argA, argB);
		break;
		case Operation::EQIR: return argB;
		break;
		case Operation::EQRI: return argA;
		break;
		case Operation::EQRR: return std::max(argA, argB);
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

	int maxIndexUsed() const
	{
		return std::max(argC, ::maxIndexUsed(op, argA, argB));
	}

	std::ostream& print(std::ostream& os) const
	{
		return os << stringify(op) << " " << int(argA)
			<< " " << int(argB) << " " << int(argC);
	}

	std::ostream& compile(std::ostream& os) const
	{
		switch (op)
		{
			case Operation::ADDR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] + reg[" << argB << "];"
					<< std::endl;
			}
			break;
			case Operation::ADDI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] + " << argB << ";"
					<< std::endl;
			}
			break;
			case Operation::MULR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] * reg[" << argB << "];"
					<< std::endl;
			}
			break;
			case Operation::MULI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] * " << argB << ";"
					<< std::endl;
			}
			break;
			case Operation::BANR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] & reg[" << argB << "];"
					<< std::endl;
			}
			break;
			case Operation::BANI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] & " << argB << ";"
					<< std::endl;
			}
			break;
			case Operation::BORR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] | reg[" << argB << "];"
					<< std::endl;
			}
			break;
			case Operation::BORI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "] | " << argB << ";"
					<< std::endl;
			}
			break;
			case Operation::SETR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "reg[" << argA << "];"
					<< std::endl;
			}
			break;
			case Operation::SETI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "" << argA << ";"
					<< std::endl;
			}
			break;
			case Operation::GTIR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(" << argA << " > reg[" << argB << "]);"
					<< std::endl;
			}
			break;
			case Operation::GTRI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(reg[" << argA << "] > " << argB << ");"
					<< std::endl;
			}
			break;
			case Operation::GTRR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(reg[" << argA << "] > reg[" << argB << "]);"
					<< std::endl;
			}
			break;
			case Operation::EQIR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(" << argA << " == reg[" << argB << "]);"
					<< std::endl;
			}
			break;
			case Operation::EQRI:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(reg[" << argA << "] == " << argB << ");"
					<< std::endl;
			}
			break;
			case Operation::EQRR:
			{
				return os
					<< "reg[" << argC << "] = "
					<< "(reg[" << argA << "] == reg[" << argB << "]);"
					<< std::endl;
			}
			break;
		}
		return os << "// unknown operation " << op << std::endl;
	}
};

std::ostream& operator<<(std::ostream& os, const Instruction& instr)
{
	return instr.print(os);
}

Instruction parseInstruction(const char* str)
{
	char o[5] = "abcd";
	int a, b, c;
	sscanf(str, "%4s %d %d %d", o, &a, &b, &c);
	return {parseOperation(o), a, b, c};
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: gec [SOURCEFILE]" << std::endl;
		return -1;
	}
	std::string srcfilename(argv[1]);
	std::string cppfilename;
	std::string codename;
	{
		size_t slashpos = srcfilename.find_last_of('/');
		std::string dirname = srcfilename.substr(0, slashpos);
		std::string basename = srcfilename.substr(slashpos + 1);
		size_t dotpos = basename.find_last_of('.');
		codename = basename.substr(0, dotpos);
		cppfilename = dirname + "/" + codename + ".h";
	}

	std::vector<Instruction> program;
	int icpos;

	{
		std::ifstream file(srcfilename);
		std::string line;

		{
			std::getline(file, line);
			sscanf(line.c_str(), "#ip %d", &icpos);
		}

		while (std::getline(file, line))
		{
			if (line.empty()) continue;

			program.emplace_back(parseInstruction(line.c_str()));
		}
	}

	int psize = program.size();
	int regsize = icpos + 1;
	for (const Instruction& instr : program)
	{
		int maxindex = instr.maxIndexUsed();
		if (maxindex >= regsize) regsize = maxindex + 1;
	}

	{
		std::ofstream file(cppfilename);

		file << "#pragma once" << std::endl;
		file << "#include <array>" << std::endl;
		file << std::endl;
		file << "#define regsize_" << codename << " " << regsize << std::endl;
		file << std::endl;
		file << "int run_" << codename << "("
			"std::array<int, regsize_" << codename << ">& reg)" << std::endl;
		file << "{" << std::endl;
		file << "\tint ticks = 0;" << std::endl;
		file << "\twhile (reg[" << icpos << "] < " << psize << ")" << std::endl;
		file << "\t{" << std::endl;
		file << "\t\tswitch (reg[" << icpos << "])" << std::endl;
		file << "\t\t{" << std::endl;

		for (size_t ic = 0; ic < program.size(); ic++)
		{
			file << "\t\tcase " << ic << ":" << std::endl;
			file << "\t\t\tticks++;" << std::endl;
			file << "\t\t\t";
			program[ic].compile(file);
			if (program[ic].argC == icpos)
			{
				file << "\t\t\tbreak;" << std::endl;
			}
			else
			{
				file << "\t\t\treg[" << icpos << "]++;" << std::endl;
			}
		}

		file << "\t\t}" << std::endl;
		file << "\t\treg[" << icpos << "]++;" << std::endl;
		file << "\t}" << std::endl;
		file << "\treturn ticks;" << std::endl;
		file << "}" << std::endl;
	}
}
