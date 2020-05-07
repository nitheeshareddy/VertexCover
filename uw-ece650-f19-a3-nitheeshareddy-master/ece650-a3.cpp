#include <string>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int rgen(int argc, char** argv) {
	argv[0] = (char*)"rgen";
	int mes = execv("rgen", argv);
	if (mes == -1) {
		std::cerr << "Error: From rgen file" << std::endl;
	}
	return 0;
}

//Entry point of Assignment2
int assignment2(void) {
	char* argv2[2];
	argv2[0] = (char*)"ece650-a2";
	argv2[1] = nullptr;
	int mes = execv("ece650-a2", argv2);
	if (mes == -1) {
		std::cerr << "Error: From a2 file" << std::endl;
	}
	return 0;

}

//Entry point of Assignment1
int assignment1(void) {
	char* argv[3];
	argv[0] = (char*)"python";
	argv[1] = (char*)"ece650-a1.py";
	argv[2] = nullptr;
	int mes = execvp("python", argv);
	if (mes == -1) {
		std::cerr << "Error: From a1 file" << std::endl;
	}
	return 0;
}

int type(void) {
	while (!std::cin.eof()) {
		string ln;
		getline(std::cin, ln);
		if (ln.size() > 0) {
			std::cout << ln << std::endl;
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	std::vector<pid_t> kids;
	int RgentoA1[2];
	pipe(RgentoA1);
	int A1toA2[2];
	pipe(A1toA2);
	pid_t child_pid;
	child_pid = fork();

	if (child_pid == 0) {
		dup2(RgentoA1[1], STDOUT_FILENO);
		close(RgentoA1[0]);
		close(RgentoA1[1]);

		//start rgen
		return rgen(argc, argv);

	}
	else if (child_pid < 0) {
		std::cerr << "Error: Unable to fork" << std::endl;
		return 1;
	}

	kids.push_back(child_pid);
	child_pid = fork();

	if (child_pid == 0) {
		dup2(RgentoA1[0], STDIN_FILENO);
		close(RgentoA1[0]);
		close(RgentoA1[1]);
		dup2(A1toA2[1], STDOUT_FILENO);
		close(A1toA2[0]);
		close(A1toA2[1]);

		//start Assignment1
		return assignment1();

	}
	else if (child_pid < 0) {
		std::cerr << "Error: Unable to fork" << std::endl;
		return 1;
	}

	kids.push_back(child_pid);
	child_pid = fork();

	if (child_pid == 0) {
		dup2(A1toA2[0], STDIN_FILENO);
		close(A1toA2[1]);
		close(A1toA2[0]);

		//start Assignment2
		return assignment2();

	}
	else if (child_pid < 0) {
		std::cerr << "Error: unable to fork" << std::endl;
		return 1;
	}

	kids.push_back(child_pid);
	child_pid = 0;
	dup2(A1toA2[1], STDOUT_FILENO);
	close(A1toA2[0]);
	close(A1toA2[1]);

	while (!std::cin.eof())
	{
		std::string line;
		std::getline(std::cin, line);
		if (line == "") {
			break;
		}
		std::cout << line << std::endl;
	}

	int result = type();
	for (pid_t k : kids) {
		int status;
		kill(k, SIGTERM);
		waitpid(k, &status, 0);
	}

	return result;
}