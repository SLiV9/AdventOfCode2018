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

int operate(const Operation& op, int argA, int argB, int* reg)
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

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: gec [SOURCEFILE]" << std::endl;
		return -1;
	}
	std::string srcfilename(argv[1]);
	std::string cppfilename;
	std::string binfilename;
	{
		size_t slashpos = srcfilename.find_last_of('/');
		std::string dirname = srcfilename.substr(0, slashpos);
		std::string basename = srcfilename.substr(slashpos + 1);
		size_t dotpos = basename.find_last_of('.');
		cppfilename = dirname + "/" + basename.substr(0, dotpos) + ".cpp";
		binfilename = dirname + "/." + basename.substr(0, dotpos);
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

	int regsize = icpos + 1;
	for (const Instruction& instr : program)
	{
		int maxindex = instr.maxIndexUsed();
		if (maxindex >= regsize) regsize = maxindex + 1;
	}

	{
		std::ofstream file(cppfilename);

		file << "//usr/bin/g++ -g -O2 "
			<< cppfilename
			<< " -o "
			<< binfilename
			<< " && time "
			<< binfilename
			<< "; exit" << std::endl;
		file << std::endl;
		file << "#include <array>" << std::endl;
		file << std::endl;
		file << "#define REGSIZE " << regsize << std::endl;
		file << std::endl;
		file << "void run(std::array<int, REGSIZE>& reg)" << std::endl;
		file << "{" << std::endl;

		// TODO

		file << "}" << std::endl;
		file << std::endl;
		file << "#ifndef MAIN" << std::endl;
		file << "#define MAIN" << std::endl;
		file << "int main(int /*argc*/, char* /*argv*/[])" << std::endl;
		file << "{" << std::endl;
		file << "  std::array<int REGSIZE> reg;" << std::endl;
		file << "  reg.fill(0);" << std::endl;
		file << "  run(reg);" << std::endl;
		file << "  std::cout << reg[0] << std::endl;" << std::endl;
		file << "}" << std::endl;
		file << "#endif" << std::endl;
	}
}
