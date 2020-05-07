#include <iostream>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <random>


struct Point
{
	int x;
	int y;
};


int orientation(Point a, Point b, Point c)
{
	int value = (b.y - a.y) * (c.x - b.x) -
		(b.x - a.x) * (c.y - b.y);

	if (value == 0) return 0;
	return (value > 0) ? 1 : 2;
}

bool validateSegment(Point a, Point b, Point c)
{
	if (b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) &&
		b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y))
		return true;

	return false;
}

bool validateSegmentOverlap(Point a1, Point b1, Point a2, Point b2)
{
	int so1 = orientation(a1, b1, a2);
	int so2 = orientation(a1, b1, b2);
	int so3 = orientation(a2, b2, a1);
	int so4 = orientation(a2, b2, b1);

	std::vector<int> x1;
	std::vector<int> x2;

	if (a1.x == a2.x && a2.x == b1.x && b1.x == b2.x) {
		x1.push_back(a1.y);
		x1.push_back(b1.y);
		x2.push_back(a2.y);
		x2.push_back(b2.y);

	}
	else {
		x1.push_back(a1.x);
		x1.push_back(b1.x);
		x2.push_back(a2.x);
		x2.push_back(b2.x);
	}
	sort(x1.begin(), x1.end());
	sort(x2.begin(), x2.end());

	if (so1 == 0 && so2 == 0 && so3 == 0 && so4 == 0) {
		if ((x1[0] >= x2[1] && x2[0] <= x1[0]) || (x2[0] >= x1[1] && x1[0] <= x2[0])) {
			return false;
		}
		return true;
	}
	return false;
}

bool validateIntersect(Point a1, Point b1, Point a2, Point b2)
{
	int so1 = orientation(a1, b1, a2);
	int so2 = orientation(a1, b1, b2);
	int so3 = orientation(a2, b2, a1);
	int so4 = orientation(a2, b2, b1);

	if (so1 != so2 && so3 != so4)
		return true;

	if (so1 == 0 && validateSegment(a1, a2, b1)) return true;
	if (so2 == 0 && validateSegment(a1, b2, b1)) return true;
	if (so3 == 0 && validateSegment(a2, a1, b2)) return true;
	if (so4 == 0 && validateSegment(a2, b1, b2)) return true;

	return false;
}

bool validateOverlap(std::vector<int> xvalue, std::vector<int> yvalue, std::vector< std::vector<int> > allxvalues, std::vector< std::vector<int> > allyvalues) {
	for (unsigned a = 1; a < xvalue.size(); a++) {

		struct Point a1 = { xvalue[a], yvalue[a] }, b1 = { xvalue[a - 1], yvalue[a - 1] };

		for (unsigned i = 0; i < allxvalues.size(); i++) {
			for (unsigned j = 1; j < allxvalues[i].size(); j++) {
				struct Point a2 = { allxvalues[i][j], allyvalues[i][j] }, b2 = { allxvalues[i][j - 1], allyvalues[i][j - 1] };
				if (validateSegmentOverlap(a1, b1, a2, b2)) {
					return true;
				}
			}
		}
	}
	return false;

}

bool validateIntersection(std::vector<int> xvalue, std::vector<int> yvalue) {
	for (unsigned i = 1; i < xvalue.size(); i++) {
		if (xvalue[i] == xvalue[i - 1] && yvalue[i] == yvalue[i - 1]) {
			return false;
		}
		else {
			continue;
		}
	}

	struct Point a1, b1, a2, b2;
	for (unsigned i = 0; i < xvalue.size() - 1; i++) {
		a1 = { xvalue[i], yvalue[i] };
		b1 = { xvalue[i + 1], yvalue[i + 1] };

		for (unsigned j = 0; j < i; j++) {
			a2 = { xvalue[j], yvalue[j] };
			b2 = { xvalue[j + 1], yvalue[j + 1] };

			if (validateIntersect(a1, b1, a2, b2) && !validateSegmentOverlap(a1, b1, a2, b2)) {
				if (xvalue[j + 1] != xvalue[i] && yvalue[j + 1] != yvalue[i]) {
					return false;
				}
				else {
					continue;
				}
			}
			else {
				continue;
			}
		}
	}
	return true;
}

std::string gererateRandomString(std::string::size_type length)
{
	static auto& chrs = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	thread_local static std::mt19937 rg{ std::random_device{}() };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

	std::string str;
	str.reserve(length);

	while (length--)
		str += chrs[pick(rg)];

	return str;
}

int ranInt() {
	std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
	if (urandom.fail()) {
		std::cerr << "Error: unable to open /dev/urandom\n";
		return 1;
	}

	unsigned int num = 0;
	unsigned int size = 1;
	urandom.read((char*)&num, size);
	urandom.close();
	return num;
}

std::vector<int> streetGenerator(int lnsegnumber, int numRange, std::vector< std::vector<int> > allxvalues, std::vector< std::vector<int> > allyvalues) {

	int countLineSegment = 0;
	std::vector<int> streetsInformation;
	int vertex;
	while (countLineSegment < (lnsegnumber + 1) * 2) {
		vertex = ranInt();
		vertex = floor(vertex * (2 * numRange) / 255) - numRange;

		if (streetsInformation.size() > 0) {
			while (vertex == streetsInformation.back()) {
				vertex = ranInt();
				vertex = floor(vertex * (2 * numRange) / 255) - numRange;
			}
		}
		streetsInformation.push_back(vertex);
		countLineSegment++;
	}

	std::vector<int> xvalue;
	std::vector<int> yvalue;
	int numberGenerated = 0;
	while (numberGenerated < 25) {
		numberGenerated++;
		for (unsigned i = 0; i < streetsInformation.size(); i++) {
			if (i % 2 == 0) {
				xvalue.push_back(streetsInformation[i]);
			}
			else {
				yvalue.push_back(streetsInformation[i]);
			}
		}
		if (validateIntersection(xvalue, yvalue) && !validateOverlap(xvalue, yvalue, allxvalues, allyvalues)) {
			return streetsInformation;
		}
		else {
			continue;
		}
	}

	if (numberGenerated >= 25) {
		xvalue.push_back(1);
		xvalue.push_back(2);
		yvalue.push_back(5);
		yvalue.push_back(7);
	}

	return streetsInformation;
}

int main(int argc, char** argv) {

	int noOfStreets = 10;
	int noOfLineSegments = 5;
	int rangeValue = 20;
	int waitTime = 5;
	int index;
	int c;
	opterr = 0;

	while ((c = getopt(argc, argv, "s:n:c:l:")) != -1)
		switch (c) {
		case 's':
			noOfStreets = std::atoi(optarg);
			break;
		case 'n':
			noOfLineSegments = std::atoi(optarg);
			break;
		case 'c':
			rangeValue = std::atoi(optarg);
			break;
		case 'l':
			waitTime = std::atoi(optarg);
			break;
		default:
			return 0;
		}

	if (optind < argc) {
		std::cout << "Found positional arguments\n";
		for (index = optind; index < argc; index++)
			std::cout << "Non-option argument: " << argv[index] << "\n";
	}

	int count = 0;
	std::string streetName;
	std::vector<std::string> streets;
	std::string result;
	std::vector<int> xvalue;
	std::vector<int> yvalue;
	std::vector<int> streetsInformation;

	while (true) {
		int realStreetNum = ranInt();
		realStreetNum = floor(realStreetNum * (noOfStreets - 2) / 255) + 2;
		int realWaitSecond = ranInt();
		realWaitSecond = floor(realWaitSecond * (waitTime - 5) / 255) + 5;
		std::vector< std::vector<int> > allxvalues;
		std::vector< std::vector<int> > allyvalues;

		while (count < realStreetNum) {
			int realLineSegmentNum = ranInt();
			realLineSegmentNum = floor(realLineSegmentNum * (noOfLineSegments - 1) / 255) + 1;
			streetsInformation = streetGenerator(realLineSegmentNum, rangeValue, allxvalues, allyvalues);

			for (unsigned i = 0; i < streetsInformation.size(); i++) {
				if (i % 2 == 0) {
					xvalue.push_back(streetsInformation[i]);
				}
				else {
					yvalue.push_back(streetsInformation[i]);
				}
			}

			for (unsigned i = 0; i < xvalue.size(); i++) {
				result.append("(");
				result.append(std::to_string(xvalue[i]));
				result.append(",");
				result.append(std::to_string(yvalue[i]));
				if (i != xvalue.size() - 1) {
					result.append(") ");
				}
				else {
					result.append(")");
				}
			}

			streetName = gererateRandomString(5);
			streets.push_back(streetName);
			streetName = "\"" + streetName + "\" ";
			std::cout << "a " << streetName << result << std::endl;
			result.clear();
			allxvalues.push_back(xvalue);
			allyvalues.push_back(yvalue);
			xvalue.clear();
			yvalue.clear();
			streetsInformation.clear();

			count++;
		}

		std::cout << "g" << std::endl;
		sleep(realWaitSecond);

		for (unsigned i = 0; i < streets.size(); i++) {
			std::cout << "r " << "\"" << streets[i] << "\"" << std::endl;
		}

		streets.clear();
		count = 0;
	}
	return 0;
}
