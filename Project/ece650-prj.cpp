#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <list>
#include <new>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <cstdio>

#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "parseInput.h"


using namespace std;

#define NUMBER_THREADS 4


struct database {
	int vertx;
	std::vector< std::pair<int, int> > edgeLst;
};

bool approx1_done = false;
bool approx2_done = false;
bool cnf_done = false;
std::vector<int> VCLstApprox1;
std::vector<int> VCLstApprox2;
std::vector<int> VCLst;


void printVertexCover()
{
	if (VCLst.empty() || VCLstApprox1.empty() || VCLstApprox2.empty())
		return;
	std::vector<int>::iterator tmp_itr;
	std::vector<int>::iterator itr;

	std::cout << "CNF-SAT-VC: ";

	for (itr = VCLst.begin(); itr != VCLst.end(); ++itr)
	{
		tmp_itr = itr;
		if (++tmp_itr == VCLst.end())
			std::cout << (*itr);
		else
			std::cout << (*itr) << ",";
	}
	std::cout << std::endl;

	std::cout << "APPROX-VC-1: ";

	for (itr = VCLstApprox1.begin(); itr != VCLstApprox1.end(); ++itr)
	{
		tmp_itr = itr;
		if (++tmp_itr == VCLstApprox1.end())
			std::cout << (*itr);
		else
			std::cout << (*itr) << ",";
	}
	std::cout << std::endl;

	std::cout << "APPROX-VC-2: ";

	for (itr = VCLstApprox2.begin(); itr != VCLstApprox2.end(); ++itr)
	{
		tmp_itr = itr;
		if (++tmp_itr == VCLstApprox2.end())
			std::cout << (*itr);
		else
			std::cout << (*itr) << ",";
	}
	std::cout << std::endl;

	return;
}


void* IOThread(void* th) {
	struct database* genGph = (struct database*)th;
	bool input_1 = true;
	bool cmd_wrong = false;
	char input_cmd = 'V';
	std::string user_input;

	while (true)
	{
		if (cnf_done && approx1_done && approx2_done)
		{
			approx1_done = false;
			approx2_done = false;
			cnf_done = false;
			printVertexCover();
			break;
		}
	}

	while (true)
	{
		switch (input_cmd) {
		case 'E':
			getline(std::cin, user_input);
			if (!std::cin) exit(0);
			cmd_wrong = parseEdge(user_input, genGph->edgeLst, genGph->vertx, &input_cmd);
			if (cmd_wrong == false)
			{
				input_1 = true;
				pthread_exit(NULL);
			}
			break;
		case 'V':
			if (input_1)
			{
				getline(std::cin, user_input);
				if (!std::cin) exit(0);
				input_1 = false;
			}
			genGph->vertx = parseVertex(user_input, &input_cmd);
			if (genGph->vertx <= 0)
				input_1 = true;
			break;
		}
	}
	pthread_exit(NULL);
}


void* VertexCoverApprox1(void* th)
{
	struct database* graphInput = (struct database*)th;
	if (graphInput->edgeLst.empty())
	{
		approx1_done = true;
		pthread_exit(NULL);
	}

	int* deg = new (std::nothrow) int[graphInput->vertx];
	if (!deg)
		std::cerr << "Error: Allocation of array degree failed." << std::endl;
	for (int i = 0; i < graphInput->vertx; ++i)
		deg[i] = 0;
	int maxDeg = 1;
	int maxDegVertex = 0;
	VCLstApprox1.clear();
	struct database genGph;
	genGph.vertx = graphInput->vertx;

	std::vector< std::pair<int, int> >::iterator it;

	for (it = graphInput->edgeLst.begin(); it != graphInput->edgeLst.end(); ++it)
	{
		genGph.edgeLst.push_back(*it);
		deg[(*it).first]++;
		deg[(*it).second]++;
		if (maxDeg <= deg[(*it).first])
		{
			maxDeg = deg[(*it).first];
			maxDegVertex = (*it).first;
		}
		if (maxDeg <= deg[(*it).second])
		{
			maxDeg = deg[(*it).second];
			maxDegVertex = (*it).second;
		}
	}

	while (!(genGph.edgeLst.empty()))
	{
		VCLstApprox1.push_back(maxDegVertex);
		maxDeg = 1;
		deg[maxDegVertex] = 0;
		for (it = genGph.edgeLst.begin(); it != genGph.edgeLst.end();)
		{
			if ((*it).first == maxDegVertex)
			{
				deg[(*it).second]--;
				genGph.edgeLst.erase(it);
			}
			else if ((*it).second == maxDegVertex)
			{
				deg[(*it).first]--;
				genGph.edgeLst.erase(it);
			}
			else
				++it;
		}

		for (it = genGph.edgeLst.begin(); it != genGph.edgeLst.end(); ++it)
		{
			if (maxDeg <= deg[(*it).first])
			{
				maxDeg = deg[(*it).first];
				maxDegVertex = (*it).first;
			}
			if (maxDeg <= deg[(*it).second])
			{
				maxDeg = deg[(*it).second];
				maxDegVertex = (*it).second;
			}
		}

	}

	std::sort(VCLstApprox1.begin(), VCLstApprox1.end(), std::less<int>());
	approx1_done = true;
	pthread_exit(NULL);
}


void* VertexCoverApprox2(void* th)
{
	struct database* graphInput = (struct database*)th;
	if (graphInput->edgeLst.empty())
	{
		approx2_done = true;
		pthread_exit(NULL);
	}

	struct database genGph;
	genGph.vertx = graphInput->vertx;
	std::vector< std::pair<int, int> >::iterator it;

	for (it = graphInput->edgeLst.begin(); it != graphInput->edgeLst.end(); ++it)
		genGph.edgeLst.push_back(*it);
	VCLstApprox2.clear();

	while (!(genGph.edgeLst.empty()))
	{
		std::pair<int, int> edges = genGph.edgeLst[0];
		VCLstApprox2.push_back(edges.first);
		VCLstApprox2.push_back(edges.second);
		for (it = genGph.edgeLst.begin(); it != genGph.edgeLst.end();)
		{
			if ((*it).first == edges.first || (*it).first == edges.second)
				genGph.edgeLst.erase(it);
			else if ((*it).second == edges.first || (*it).second == edges.second)
				genGph.edgeLst.erase(it);
			else
				it++;
		}
	}

	std::sort(VCLstApprox2.begin(), VCLstApprox2.end(), std::less<int>());
	approx2_done = true;
	pthread_exit(NULL);
}


void* VertexCoverCNF(void* th)
{
	struct database* genGph = (struct database*)th;

	if (genGph->edgeLst.empty())
	{
		cnf_done = true;
		pthread_exit(NULL);
	}
	int p = genGph->vertx;
	std::vector< std::pair<int, int> >& edges = genGph->edgeLst;
	std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
	int low = 0;
	int high = p;
	int l = 0;
	bool re = false;

	while (low < high)
	{
		l = (low + high) / 2;

		std::vector< std::vector< Minisat::Lit > > lit(p);

		for (int i = 0; i < p; ++i)
		{
			for (int j = 0; j < l; ++j)
			{
				Minisat::Lit tmpLit = Minisat::mkLit(solver->newVar());
				lit[i].push_back(tmpLit);
			}
		}

		for (int i = 0; i < l; ++i)
		{
			Minisat::vec<Minisat::Lit> tmpVector;
			for (int j = 0; j < p; ++j)
				tmpVector.push(lit[j][i]);
			solver->addClause(tmpVector);
			tmpVector.clear();
		}

		for (int i = 0; i < p; ++i)
		{
			for (int j = 0; j < l - 1; ++j)
			{
				for (int k = j + 1; k < l; ++k)
					solver->addClause(~lit[i][j], ~lit[i][k]);
			}
		}

		for (int i = 0; i < l; ++i)
		{
			for (int j = 0; j < p - 1; ++j)
			{
				for (int k = j + 1; k < p; ++k)
					solver->addClause(~lit[j][i], ~lit[k][i]);
			}
		}

		std::vector< std::pair<int, int> >::iterator it;

		for (it = edges.begin(); it != edges.end(); ++it)
		{
			Minisat::vec<Minisat::Lit> tmpVector;
			for (int i = 0; i < l; ++i)
			{
				tmpVector.push(lit[(*it).first][i]);
				tmpVector.push(lit[(*it).second][i]);
			}
			solver->addClause(tmpVector);
			tmpVector.clear();
		}
		
		re = solver->solve();
		if (re == 1)
			high = l;
		else if (re == 0)
			low = l + 1;
		lit.clear();
		solver.reset(new Minisat::Solver());
	}
	l = high;

	std::vector< std::vector< Minisat::Lit > > lit(p);

	for (int i = 0; i < p; ++i)
	{
		for (int j = 0; j < l; ++j)
		{
			Minisat::Lit tmpLit = Minisat::mkLit(solver->newVar());
			lit[i].push_back(tmpLit);
		}
	}

	for (int i = 0; i < l; ++i)
	{
		Minisat::vec<Minisat::Lit> tmpVector;
		for (int j = 0; j < p; ++j)
			tmpVector.push(lit[j][i]);
		solver->addClause(tmpVector);
		tmpVector.clear();
	}

	for (int i = 0; i < p; ++i)
	{
		for (int j = 0; j < l - 1; ++j)
		{
			for (int k = j + 1; k < l; ++k)
				solver->addClause(~lit[i][j], ~lit[i][k]);
		}
	}

	for (int i = 0; i < l; ++i)
	{
		for (int j = 0; j < p - 1; ++j)
		{
			for (int k = j + 1; k < p; ++k)
				solver->addClause(~lit[j][i], ~lit[k][i]);
		}
	}

	std::vector< std::pair<int, int> >::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it)
	{
		Minisat::vec<Minisat::Lit> tmpVector;
		for (int i = 0; i < l; ++i)
		{
			tmpVector.push(lit[(*it).first][i]);
			tmpVector.push(lit[(*it).second][i]);
		}
		solver->addClause(tmpVector);
		tmpVector.clear();
	}
	re = solver->solve();
	VCLst.clear();

	for (int i = 0; i < p; ++i)
	{
		for (int j = 0; j < l; ++j)
		{
			if (solver->modelValue(lit[i][j]) == Minisat::l_True)
				VCLst.push_back(i);
		}
	}

	std::sort(VCLst.begin(), VCLst.end(), std::less<int>());
	lit.clear();
	solver.reset(new Minisat::Solver());
	cnf_done = true;
	pthread_exit(NULL);
}


int main()
{
	struct database genGph;
	pthread_t thrd[NUMBER_THREADS];
	pthread_attr_t attribute;
	void* status;
	int ct;
	int i;

	while (true)
	{
		pthread_attr_init(&attribute);
		pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

		ct = pthread_create(&thrd[0], &attribute, VertexCoverCNF, (void*)&genGph);
		if (ct) {
			cout << "Error: Thread Creation Failed," << ct << endl;
			exit(-1);
		}

		ct = pthread_create(&thrd[1], &attribute, VertexCoverApprox1, (void*)&genGph);
		if (ct) {
			cout << "Error: Thread Creation Failed," << ct << endl;
			exit(-1);
		}

		ct = pthread_create(&thrd[2], &attribute, VertexCoverApprox2, (void*)&genGph);
		if (ct) {
			cout << "Error: Thread Creation Failed," << ct << endl;
			exit(-1);
		}

		ct = pthread_create(&thrd[3], &attribute, IOThread, (void*)&genGph);
		if (ct) {
			cout << "Error: Thread Creation Failed," << ct << endl;
			exit(-1);
		}

		pthread_attr_destroy(&attribute);
		for (i = 0; i < NUMBER_THREADS; i++) {
			ct = pthread_join(thrd[i], &status);
			if (ct) {
				cout << "Error: Unable to join," << ct << endl;
				exit(-1);
			}
		}
	}
	return 0;
}