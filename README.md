# VertexCover

rgen.cpp - It generates the random inputs for the python file a1-ece650.py. GenerateCoordinates.h - Header file for GenerateCoordinates.cpp. GenerateCoordinates.cpp - It generates random coordinates of the streets. ValidateCoordinates.h - Header file for ValidateCoordinates.cpp. ValidateCoordinates.cpp - It Validates the output of the rgen. It removes the circular paths, overlapping paths, partial overlapping paths, duplicate paths and any misleading paths.

a1-ece650.py - It is the python file which takes input from the rgen and generate the graph with intersection points as the nodes and the roads as the edges between the nodes. ValidateInput.py - It is the python file validating the input from the rgen.

a2-ece650.cpp - It is the main file for generating the shortest path between the two nodes. TakeInput.h - Header file for TakeInput.cpp. TakeInput.cpp - This file validates the input from python file. FindShortestPath.h - Header file for FindShortestPath.cpp. FindShortestPath.cpp - This file finds the shortest path between the given vertices. It uses the breadth first search algorithm to find the shortest path.

a3-ece650.cpp - It is the driver program connect rgen with a1-ece650 and a1-ece650 with a2-ece650.

a4-ece650.cpp - This is the main file which calls the FindVertexCover.cpp file. FindVertexCover.h - It is the header file for FindVertexCover.cpp. FindVertexCover.cpp - This file finds the vertex cover of the given vertices and the edges using sat solver.

prj-ece650 - This file uses threads and compare three different sat solves: CNF SAT solver, Approx-1, Approx-2.

doctest.h - Header file for test.cpp. test.cpp - It is the test file. CMakeLists.txt - It generates the executables for .cpp files. README.md - It contains the information about the files for the project.
