//usr/bin/g++ -g -O2 dec8/two.cpp -o dec8/.two && time ./dec8/.two; exit

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cassert>



static int _stored = -1;

void store(int x)
{
	_stored = x;
}

bool retrieve(int* x)
{
	if (_stored < 0) return false;

	*x = _stored;
	_stored = -1;
	return true;
}

struct Node
{
	size_t nodecount = size_t(-1);
	size_t metacount = size_t(-1);
	std::vector<Node> nodes;
	std::vector<int> metas;

	int sum() const
	{
		int x = 0;
		for (const Node& node : nodes)
		{
			x += node.sum();
		}
		for (int meta : metas)
		{
			x += meta;
		}
		return x;
	}

	int value() const
	{
		int x = 0;
		if (nodes.empty())
		{
			for (int meta : metas)
			{
				x += meta;
			}
		}
		else
		{
			for (int index : metas)
			{
				if (index <= 0) continue;
				size_t i = index - 1;
				if (i >= nodes.size()) continue;
				x += nodes[i].value();
			}
		}
		return x;
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	std::ifstream file("dec8/input.txt");

	std::string line;
	std::getline(file, line);
	std::stringstream strm(line);

	Node root;
	std::vector<Node*> stack;
	stack.emplace_back(&root);

	int x;
	while (retrieve(&x) || strm >> x)
	{
		if (stack.empty()) break;
		Node& cur = *(stack.back());

		if (cur.nodecount == size_t(-1))
		{
			cur.nodecount = x;
			cur.nodes.reserve(x);
		}
		else if (cur.metacount == size_t(-1))
		{
			cur.metacount = x;
			cur.metas.reserve(x);
		}
		else if (cur.nodes.size() < cur.nodecount)
		{
			cur.nodes.emplace_back();
			cur.nodes.back().nodecount = x;
			stack.emplace_back(&(cur.nodes.back()));
		}
		else if (cur.metas.size() < cur.metacount)
		{
			cur.metas.emplace_back(x);
		}
		else
		{
			stack.pop_back();
			store(x);
		}
	}

	std::cout << root.value() << std::endl;
}
