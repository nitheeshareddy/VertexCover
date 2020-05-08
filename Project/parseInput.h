#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <regex>


int parseVertex(std::string, char*);
bool parseEdge(std::string, std::vector< std::pair<int, int> >&, int, char*);
std::string removeCharac(std::string, char);