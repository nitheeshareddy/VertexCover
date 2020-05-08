#include "parseInput.h"


using namespace std;

int parseVertex(string user_input, char* input_cmd)
{
	int vertx;
	regex edge_input("^[[:blank:]]*E{1}[[:blank:]]+\\{{1}([<>0-9, ]+)\\}{1}[[:blank:]]*$");
	regex vertex_input("^[[:blank:]]*V{1}[[:blank:]]+([[:digit:]]+)[[:blank:]]*$");
	regex shortest_input("^[[:blank:]]*s{1}[[:blank:]]+([[:digit:]]+)[[:blank:]]+([[:digit:]]+)[[:blank:]]*$");
	regex edge_parsed("<(\\d+),(\\d+)>,{0,1}");
	smatch n;

	while (true)
	{
		if (regex_match(user_input, vertex_input))
		{
			regex_search(user_input, n, vertex_input);
			stringstream vertex(n[1].str());
			vertex >> vertx;
			if (vertx > 0)
			{
				*input_cmd = 'E';
				return vertx;
			}
			else
			{
				cout << "Error: No. of vertices should be atleast 1." << endl;
				return 0;
			}
		}
		else
		{
			cout << "Error: Invalid input of vertex!" << endl;
			return 0;
		}
	}

}

string removeCharac(string st, char remo)
{
	st.erase(remove(st.begin(), st.end(), remo), st.end());
	return st;
}

bool parseEdge(string user_input, vector< pair<int, int> >& edge_pairs, int vertx, char* input_cmd)
{
	regex edge_input("^[[:blank:]]*E{1}[[:blank:]]+\\{{1}([<>0-9, ]+)\\}{1}[[:blank:]]*$");
	regex vertex_input("^[[:blank:]]*V{1}[[:blank:]]+([[:digit:]]+)[[:blank:]]*$");
	regex shortest_input("^[[:blank:]]*s{1}[[:blank:]]+([[:digit:]]+)[[:blank:]]+([[:digit:]]+)[[:blank:]]*$");
	regex edge_parsed("<(\\d+),(\\d+)>,{0,1}");
	smatch n;
	string edge_str;

	while (true)
	{
		if (regex_match(user_input, edge_input))
		{
			regex_search(user_input, n, edge_input);
			edge_str = removeCharac(n[1].str(), ' ');
			sregex_iterator itr(edge_str.begin(), edge_str.end(), edge_parsed);
			sregex_iterator itr_end;
			int edge = 0;
			int vertx1, vertx2;
			int length_str1 = edge_str.length();
			int length_str2 = 0;
			edge_pairs.clear();
			
			while (itr != itr_end)
			{
				std::smatch matches = *itr;
				string temp = matches.str();
				regex_search(temp, n, edge_parsed);
				stringstream vertx1str(n[1].str());
				stringstream vertx2str(n[2].str());
				vertx1str >> vertx1;
				vertx2str >> vertx2;
				if ((vertx1 != vertx2) && (vertx1 >= 0) && (vertx2 >= 0) && (vertx1 < vertx) && (vertx2 < vertx))
					edge_pairs.push_back(make_pair(vertx1, vertx2));
				else break;
				length_str2 += (matches.str()).length();
				edge++;
				++itr;
			}
			if (length_str1 == length_str2)
			{
				*input_cmd = 'V';
				return false;
			}
			else
			{
				cout << "Error: Invalid input of edge!" << endl;
				return true;
			}
		}
		else if (regex_match(user_input, vertex_input))
		{
			*input_cmd = 'V';
			return true;
		}
		else
		{
			cout << "Error: Invalid input of edge!" << endl;
			return true;
		}

	}

}