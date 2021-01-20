//Jasper Hatton, 2020

#include "graph.h"
#include <iterator>
#include <fstream>

using namespace std;

template <class T>
Gnode<T>::Gnode(int id, T cost):id(id),cost(cost){}

//Allow nodes to be compared with ==
template <class T>
bool operator== (Gnode<T> x, Gnode<T> y){
	if(x.id==y.id && x.cost==y.cost){return 1;}
	else{return 0;}
}

//Functions to allow easy output of nodes
template <class T>
ostream& operator<< (ostream& out, Gnode<T>& n){
	out << "(" << n.id << "," << n.cost << ")";
	return out;
}

//Constructor for an empty graph
template <class T>
Graph<T>::Graph(int size):size(size),adjlist(size){}

//Return a list of neighbours given the index of a graph element
template <class T>
list<Gnode<T> > Graph<T>::getneighbours(int x){return adjlist.at(x);}

//Add an edge to the graph
template <class T>
void Graph<T>::addedge(int s, int d, T cost, bool directed){
	Gnode<T>* newnode=new Gnode<T>;
	
	//Convert the edge to a node and add it into the adjacency list (twice for undirectedness)
	newnode->id=d;
	newnode->cost=cost;
	adjlist.at(s).push_back(*newnode);
	
	if(!directed){
		newnode->id=s;
		adjlist.at(d).push_back(*newnode);
	}
	
	delete newnode;
}

template Gnode<int>::Gnode(int, int);
template bool operator== <int> (Gnode<int>, Gnode<int>);
template ostream& operator<< <int> (ostream& out, Gnode<int>&);
template Graph<int>::Graph(int);
template void Graph<int>::addedge(int,int,int,bool);
template list<Gnode<int> > Graph<int>::getneighbours(int);