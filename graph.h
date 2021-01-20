//Jasper Hatton, 2020

#include <vector>
#include <list>
#include <iostream>

template <class T> class Graph;

//Class containing the id of the next node and the cost to it, but not the id of source node
template <class T>
class Gnode{	
public:
	Gnode(int id=0, T cost=1);
	int id;
	T cost;
};

template <class T>
class Graph{
public:
	//Constructor for an empty graph
	Graph(int size);
	
	//Return a list of neighbours given the index of a graph element
	std::list<Gnode<T> > getneighbours(int x);
	
	int getsize() const;
	
	void addedge(int s, int d, T cost=1, bool directed=1);
	
protected:
	std::vector<std::list<Gnode<T> > > adjlist; //the data for the graph is stored as an array containing lists of (next node id#,edge weight)
	int size;
};

template <class T>
std::ostream& operator<< (std::ostream& out, Gnode<T>& n);

template <class T>
bool operator== (Gnode<T> x, Gnode<T> y);

