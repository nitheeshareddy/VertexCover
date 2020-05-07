#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <cstring>
#include <list>
#include <minisat/core/Solver.h>
#include <iostream>

class GenGraph
{
	int V;
public:
	std::list<int>* adjacent;
	GenGraph(int V);
	void addEdges(int v, int w);
};

void GenGraph::addEdges(int a, int b)
{
	adjacent[a].push_back(b);
	adjacent[b].push_back(a);
}

GenGraph::GenGraph(int V)
{
	this->V = V;
	adjacent = new std::list<int>[V];
}

std::string replace(std::string str, const std::string& replace, char ch) {
	size_t found = str.find_first_of(replace);

	while (found != std::string::npos) {
		str[found] = ch;
		found = str.find_first_of(replace, found + 1);
	}
	return str;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t position_start = 0, position_end, delimeter_length = delimiter.length();
	std::string token;
	std::vector<std::string> result;

	while ((position_end = s.find(delimiter, position_start)) != std::string::npos) {
		token = s.substr(position_start, position_end - position_start);
		position_start = position_end + delimeter_length;
		result.push_back(token);
	}

	result.push_back(s.substr(position_start));
	return result;
}

std::vector<int> vertexCover(int noOfVertices, int noOfVertexCover, std::vector<int> noVertices) {
	Minisat::Solver sol;
	std::vector <std::vector<Minisat::Lit>> matrix(noOfVertices);
	for (int s = 0; s < noOfVertices; ++s) {
		for (int l = 0; l < noOfVertexCover; ++l) {
			Minisat::Lit literal = Minisat::mkLit(sol.newVar());
			matrix[s].push_back(literal);
		}
	}
	for (int l = 0; l < noOfVertexCover; ++l) {
		Minisat::vec<Minisat::Lit> literals;
		for (int s = 0; s < noOfVertices; ++s) {
			literals.push(matrix[s][l]);
		}
		sol.addClause(literals);
		literals.clear();
	}
	for (int s = 0; s < noOfVertices; ++s) {
		for (int p = 0; p < noOfVertexCover - 1; ++p) {
			for (int q = p + 1; q < noOfVertexCover; ++q) {
				sol.addClause(~matrix[s][p], ~matrix[s][q]);
			}
		}
	}
	for (int m = 0; m < noOfVertexCover; ++m) {
		for (int p = 0; p < noOfVertices - 1; ++p) {
			for (int q = p + 1; q < noOfVertices; ++q) {
				sol.addClause(~matrix[p][m], ~matrix[q][m]);
			}
		}
	}
	std::vector<int> before;
	std::vector<int> after;
	for (int r = 0; r < noVertices.size(); ++r) {
		if (r % 2 == 0) {
			before.push_back(noVertices[r]);
		}
		else {
			after.push_back(noVertices[r]);
		}
	}
	for (int r = 0; r < before.size(); ++r) {
		Minisat::vec<Minisat::Lit> literals;
		for (int l = 0; l < noOfVertexCover; ++l) {
			literals.push(matrix[before[r]][l]);
			literals.push(matrix[after[r]][l]);
		}
		sol.addClause(literals);
		literals.clear();
	}
	auto sat = sol.solve();
	if (sat) {
		std::vector<int> result;
		for (int r = 0; r < noOfVertices; r++) {
			for (int j = 0; j < noOfVertexCover; ++j) {
				if (Minisat::toInt(sol.modelValue(matrix[r][j])) == 0) {
					result.push_back(r);
				}
			}
		}
		return result;
	}
	else {
		return { -1 };
	}
}

void graph(std::string allnodes, int number, GenGraph graph) {
	std::string edges;
	bool flag = true;
	edges = replace(allnodes, "<", ' ');
	edges = replace(edges, ">", ' ');
	edges = replace(edges, "{", ' ');
	edges = replace(edges, "}", ' ');
	edges.erase(remove_if(edges.begin(), edges.end(), isspace), edges.end());
	std::vector<std::string> vertices = split(edges, ",");
	std::vector<int> noVertices;
	for (auto& a : vertices) {
		std::stringstream parser(a);
		int k = 0;
		parser >> k;
		noVertices.push_back(k);
	}
	for (unsigned in = 0; in < vertices.size(); ++in) {
		if (in % 2 == 0) {
			if ((std::stoi(vertices[in]) >= number) || (std::stoi(vertices[in + 1]) >= number)) {
				std::cerr << "Error: Invalid input! Edge specified is out of range of no of vertices" << std::endl;
				flag = false;
			}
			else {
				graph.addEdges(std::stoi(vertices[in]), std::stoi(vertices[in + 1]));
			}

		}
		else {
			continue;
		}
	}
	if (flag) {
		int numVertices = number;
		std::vector<int> res;
		std::vector<int> endRes;
		std::vector<int> temp;
		temp = { -1 };
		int d = 1;
		int c = numVertices;
		while (d <= c) {
			int mid = floor((d + c) / 2);
			res = vertexCover(numVertices, mid, noVertices);
			bool checkResult = std::equal(res.begin(), res.end(), temp.begin());
			if (not checkResult) {
				c = mid - 1;
				endRes.clear();
				endRes = res;
			}
			else {
				d = mid + 1;
			}
		}
		sort(endRes.begin(), endRes.end());
		for (int i = 0; i < endRes.size(); ++i) {
			std::cout << endRes[i] << " ";
		}
		std::cout << "\n";
	}
}

int main() {
	GenGraph g(0);
	using Minisat::mkLit;
	using Minisat::lbool;
	int value = 0;
	while (!std::cin.eof()) {
		std::vector<std::string> tokens;
		std::string string_input;
		std::getline(std::cin, string_input);
		if (std::cin.eof()) {
			break;
		}
		tokens = split(string_input, " ");
		if (tokens.size() == 2) {
			if (tokens[0] == "V") {
				value = std::stoi(tokens[1]);
				g = GenGraph(value);
			}
			else if (tokens[0] == "E") {
				graph(tokens[1], value, g);
			}
			else {
				std::cout << "Error: Invalid input.Please provide a valid input!" << std::endl;
			}
		}
		else {
			continue;
		}
	}
	return 0;
}