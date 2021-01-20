//Jasper Hatton, 2020
//Compile program with g++ hex.cc graph.cc main.cc -std=c++11

#include "hex.h"
#include <iostream>

using namespace std;

int main(){
	int size=7;
	Hex* game1;
	bool win;
	string end_msg;
	long int sims;
	string size_msg="Enter the board size: ";
	string sims_msg="Enter the number of simulations per move: ";
	
	get_input(size,size_msg);
	
	//get_input(sims,sims_msg);
	sims=8000;
	
	game1=new Hex(size,sims); 
	
	win=game1->start();
	
	if(win){end_msg="You Won!";	}
	else{end_msg="Better luck next time...";}
		
	cout<<endl<<end_msg<<endl;
	
	return 0;
//	*/
}