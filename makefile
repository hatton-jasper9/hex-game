hex.exe: main.o hex.o graph.o
	g++ -g -o hex.exe -std=c++11 -pthread main.o hex.o graph.o
	
main.o: main.cc hex.h graph.h
	g++ -g -c -std=c++11 main.cc

hex.o: hex.cc hex.h graph.h
	g++ -g -c -std=c++11 -pthread hex.cc

graph.o: graph.h graph.cc
	g++ -g -c -std=c++11 graph.cc