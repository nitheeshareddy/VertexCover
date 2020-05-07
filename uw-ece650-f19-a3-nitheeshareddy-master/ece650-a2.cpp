#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <list>
#include <set>
#include <cstring>
#include <sstream>


// replace specific character into a specific character
std::string replace(std::string str, const std::string& replace, char ch) {
	size_t found = str.find_first_of(replace);

	while (found != std::string::npos) {
		str[found] = ch;
		found = str.find_first_of(replace, found + 1);
	}
	return str;
}

// To split the string
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

class GenGraph {
	int V;

public:
	std::list<int> vertex_list;
	std::list<int>* adjacent;
	GenGraph(int V);
	void addEdges(int v, int w);
	std::list<int> FindShortestPath(int s, int e);
	std::list<int> getvert_list();
};

GenGraph::GenGraph(int V)
{
	this->V = V;
	adjacent = new std::list<int>[V];
}

void GenGraph::addEdges(int a, int b)
{
	adjacent[a].push_back(b);
	adjacent[b].push_back(a);
	vertex_list.push_back(a);
	vertex_list.push_back(b);
}
std::list<int> GenGraph::getvert_list() {
	return vertex_list;
}

std::list<int> GenGraph::FindShortestPath(int src, int dst)
{
	bool* visited = new bool[V];
	std::fill_n(visited, V, 0);
	std::list<int> res;
	std::list<int> path;
	std::list<int> res_path;
	std::list<int> neighbours;

	path.push_back(src);

	std::vector<std::list<int> > queue;
	int node;
	int temp;

	if (src == dst) {
		res.push_back(src);
		res.push_back(dst);
		return res;
	}

	queue.push_back(path);
	std::list<int>::iterator i;

	while (!queue.empty())
	{
		path = queue.front();
		queue.erase(queue.begin());
		node = path.back();

		if (!visited[node])
		{
			neighbours = adjacent[node];
			for (auto& neighbour : neighbours)
			{
				res_path = path;
				temp = neighbour;
				res_path.push_back(temp);
				queue.push_back(res_path);

				if (temp == dst)
				{
					return res_path;
				}
			}
			visited[node] = 1;
		}
	}
	return res;
}

void pathGenerator(std::vector<std::string> tokens, GenGraph g) {
	std::list<int> path_validation;
	std::list<int>::const_iterator iterator;
	int from;
	int to;
	bool from_flg = false;
	bool to_flg = false;
	from = std::stoi(tokens[1]);
	to = std::stoi(tokens[2]);
	std::list<int> vertex_list = g.getvert_list();

	for (int x : vertex_list) {
		if (x == from) {
			from_flg = true;
		}
		if (x == to) {
			to_flg = true;
		}
	}
	if (from_flg && to_flg) {

		path_validation = g.FindShortestPath(from, to);
	}
	std::string res;

	std::vector<int> vertex{ std::begin(path_validation), std::end(path_validation) };

	for (unsigned i = 0; i < vertex.size(); ++i)
	{
		if (i != 0)
			res.append("-");
		res.append(std::to_string(vertex[i]));
	}


	if (!path_validation.empty()) {
		std::cout << res << std::endl;
	}
	else {
		std::cerr << "Error: Provided vertex doesnot exist or path doesnot exist between the given two vertices." << std::endl;
	}
}

GenGraph graph(std::string allnodes, int number, GenGraph graph) {
	std::string edges;
	edges = replace(allnodes, "<", ' ');
	edges = replace(edges, ">", ' ');
	edges = replace(edges, "{", ' ');
	edges = replace(edges, "}", ' ');
	edges.erase(remove_if(edges.begin(), edges.end(), isspace), edges.end());

	std::vector<std::string> vertices = split(edges, ",");

	for (unsigned index = 0; index < vertices.size(); ++index) {
		if (index % 2 == 0) {
			if ((std::stoi(vertices[index]) >= number) || (std::stoi(vertices[index + 1]) >= number)) {
				std::cerr << "Error: Invalid input! Given Vertex id is larger compared to the size of the graph." << std::endl;
			}
			else {
				graph.addEdges(std::stoi(vertices[index]), std::stoi(vertices[index + 1]));
			}
		}
		else {
			continue;
		}
	}
	return graph;
}

int main() {
	GenGraph g(0);

	while (!std::cin.eof()) {
		//char input[256];
		std::string string_input;
		std::vector<std::string> tokens;
		int value;
		//std::list<int> mylist;
		getline(std::cin, string_input);

		if (std::cin.eof()) {
			break;
		}

		//string_input = std::string(input);

		tokens = split(string_input, " ");

		if (tokens.size() == 2) {
			if (tokens[0] == "V") {
				std::cout << string_input << std::endl;
				value = std::stoi(tokens[1]);
				g = GenGraph(value);
			}
			else if (tokens[0] == "E") {
				// std::cout<<tokens[1].length()<<std::endl;
				std::cout << string_input << std::endl;
				if (tokens[1].length() > 2) {
					g = graph(tokens[1], value, g);
				}
			}
			else {
				std::cerr << "Error: Invalid input from A2!" << std::endl;
			}
		}
		else if (tokens.size() == 3 && tokens[0] == "s") {
			//pathGenerator(tokens, g);
		//}
			if (std::stoi(tokens[1]) >= value || std::stoi(tokens[2]) >= value) {
				std::cerr << "Error: Path doesnot exist between these two vertices" << std::endl;
			}
			else {
				pathGenerator(tokens, g);
			}
		}
		else {
			//std::cerr << "Error: Invalid input.Please provide a valid input." << std::endl;
			continue;
		}
	}
	return 0;
}