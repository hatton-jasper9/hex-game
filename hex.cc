//Jasper Hatton, 2020

#include "hex.h"
#include <algorithm>
#include <thread>
#include <cstdint>
#include <utility>
#include <random>
#include <cassert>
#include <cmath>

//#define NDEBUG

using namespace std;

//Constructor allocates status vector, constructs graph, adds the connections of a hex board
Hex::Hex(int size, long int trials):size(size),num_mc_trials(trials),hex_status(size*size,Status::OPEN),Graph(size*size){
	for(int i=0;i<Graph::size;i++){
		if(top(i) && left_side(i)){
			addedge(i,i+1);
			addedge(i,i+size);
		}
		else if (top(i) && right_side(i)){
			addedge(i,i-1);
			addedge(i,i+size);
			addedge(i,i+size-1);
		}
		else if (bottom(i) && left_side(i)){
			addedge(i,i+1);
			addedge(i,i-size);
			addedge(i,i-size+1);
		}
		else if (bottom(i) && right_side(i)){
			addedge(i,i-1);
			addedge(i,i-size);
		}
		else if(left_side(i)){
			addedge(i,i-size+1);
			addedge(i,i+1);
			addedge(i,i+size);
			addedge(i,i-size);
		}
		else if(right_side(i)){
			addedge(i,i+size-1);
			addedge(i,i-1);
			addedge(i,i-size);
			addedge(i,i+size);
		}
		else if(top(i)){
			addedge(i,i+1);
			addedge(i,i-1);
			addedge(i,i+size);
			addedge(i,i+size-1);
		}
		else if(bottom(i)){
			addedge(i,i+1);
			addedge(i,i-1);
			addedge(i,i-size);
			addedge(i,i-size+1);
		}
		else{
			addedge(i,i+1);
			addedge(i,i-1);
			addedge(i,i+size);
			addedge(i,i-size+1);
			addedge(i,i-size);
			addedge(i,i+size-1);
		}
	}
}

//Defining my inline functions
inline bool Hex::left_side(int x) const {return get_col(x)==0;}
inline bool Hex::right_side(int x) const {return get_col(x)==size-1;}
inline bool Hex::top(int x) const {return get_row(x)==0;}
inline bool Hex::bottom(int x) const {return get_row(x)==size-1;}
inline int Hex::get_col(int x) const {return x%size;}
inline int Hex::get_row(int x) const {return x/size;}

//Prints out the board for the user
void Hex::display(){
	display(hex_status);
}

//Display a board based on the given status vector, which shows the status of each tile.
//Used to be able to display the board inside mc_trial, because each mc_trial has its own status vector
void Hex::display(vector<Status> hex_status){
	cout<<"  ";
	
	//Print number labels for the columns
	for(int c=0;c<size;c++){
		cout<<c+1;
		assert(3-log10(c+1) > 0);
		for(int i=0;i<3-log10(c+1);i++){cout<<" ";}
	}
	
	cout<<endl;
	
	for(int r=0;r<size;r++){
		
		//Print the number label for the row
		cout<<r+1<<" ";
		
		//Add the padding spaces for the row
		assert(r*2-log10(r+1)>=0);
		for(int i=0;i<r*2-log10(r+1);i++){cout << " ";}
		
		//Print the row, including dashed between tiles
		for(int c=0;c<size;c++){
			cout<<hex_status.at(r*size+c);
			if(c!=size-1){cout << "- ";}
		}
		
		cout << endl;
		
		//Print the row of shlashes connecting to the next row of tiles
		if(r!=size-1){
			for(int i=0;i<r*2+3;i++){cout << " ";}
			cout << "\\ ";
			for(int i=0;i<size-1;i++){cout<<"/ \\ ";}
			cout<<endl;
		}
	}
	
	cout<<endl;
}

//Make the move, if legal. If not do nothing and returns 1.
//Offests x and y are added to row and col if the play goes through.
bool Hex::play(Player player, int& row, int& col,int x, int y){
	int i=row+x;
	int j=col+y;
	
	if(i>=0 && i<size && j>=0 && j<size){
		if(hex_status.at(i*size+j)==Status::OPEN){
			hex_status.at(i*size+j)=static_cast<Status>(player);
			row=i;
			col=j;
			return 0;
		}
		else{return 1;}
	}
	else{return 1;}
}

//Output for a tile
ostream& operator<<(ostream& out, Status& x){
	switch(x){
		case Status::OPEN: out << ". "; break;
		case Status::BLUE: out << "X "; break;
		case Status::RED: out << "O "; break;
	}
	return out;
}

//Input for a player
istream& operator>>(istream& in, Player& x){
	char a;
	
	in>>a;
	
	if(a=='B' || a=='b'){x=Player::BLUE;}
	if(a=='R' || a=='r'){x=Player::RED;}
	
	return in;
}

//Input for a player
istream& operator>>(istream& in, pair<int,int>& x){
	in>>x.first>>x.second;
	
	return in;
}

//Used to get a players opponent easily. ! operator is natural because Player is a binary enum.
Player operator!(Player player){
	if(player==Player::BLUE){return Player::RED;}
	if(player==Player::RED){return Player::BLUE;}
}

//Allows the player to enter a move. Loops until the move is legal. Returns 1 if the move results in a win, 0 otherwise.
bool Hex::human_turn(){
	int row;
	int col;
	int r;
	int x;
	Player player=human_player;
	vector<bool> checked(Graph::size, 0);
	string input_msg="Enter move as <row> <column>: ";
	pair<int,int> coordinates;
	
	get_input(coordinates,input_msg);
	row=coordinates.first;
	col=coordinates.second;
	
	r=play(player,--row,--col);
	
	//Ask for a new move until a valid move is entered (r==0)
	while(r==1){
		cout<<"Move not valid! Try again. \n \n";
		
		get_input(coordinates,input_msg);
		row=coordinates.first;
		col=coordinates.second;
		
		r=play(player,--row,--col);
	}
	
	x=row*size+col;
	
	return iswinning(x,hex_status,checked);
}

//Checks if the move made on the co-ordinate x is a winning move. Basic graph traversal starting at position x with flags for reaching each of the target edges.
bool Hex::iswinning(int x, vector<Status> status_vec, vector<bool>& checked){
	list<int> open; //Stores the reachable tiles
	int selected;
	list<Gnode<int> > neighbours;
	Gnode<int>* next_neighbour=new Gnode<int>;
	Player player=static_cast<Player>(status_vec.at(x)); //The player whoe made the move is read from the status of the tile at x
	Side side1; //The first target side
	Side side2;
	bool is_side1=0; //Has side1 been reached yet?
	bool is_side2=0;
	
	assert(checked.size()==Graph::size);
	assert(player==Player::RED || player==Player::BLUE);
	
	//Set the destination sides based on the player
	if(player==Player::BLUE){
		side1=Side::LEFT;
		side2=Side::RIGHT;
	}
	else{
		side1=Side::TOP;
		side2=Side::BOTTOM;
	}
		
	//Add the inputted tile to the list of tiles
	open.push_front(x);
	checked.at(x)=1;
	
	while(!open.empty()){
		//Take from the list of tiles
		selected=open.front();
		open.pop_front();
		
		//Check if the selected tile is on a target side and set the flags.
		if(side1==Side::LEFT && left_side(selected) || side1==Side::TOP && top(selected)){is_side1=1;}
		if(side2==Side::RIGHT && right_side(selected) || side2==Side::BOTTOM && bottom(selected)){is_side2=1;}
		
		//Check conditions for a realized win (both sides are reached by the continuous chain)
		if(is_side1&&is_side2){return 1;}
		
		//Get neigbours
		neighbours = getneighbours(selected);
		
		//If the selected tile has new neighbours which are the player's tiles, add them on to list "open"
		while(!neighbours.empty()){
			*next_neighbour=neighbours.back();
			neighbours.pop_back();
			if(!checked.at(next_neighbour->id) && status_vec.at(next_neighbour->id)==static_cast<Status>(player) && status_vec.at(next_neighbour->id)!=Status::OPEN){
				open.push_front(next_neighbour->id);
				checked.at(next_neighbour->id)=1;
			}
		}
	}
		
	delete next_neighbour;
	return 0;
}

//ai_turn chooses the move with the highest number of mc_trials returning a win, and returns 1 if the selected move is winning
bool Hex::ai_turn(){
	long wins1=0;
	long wins2=0;
	vector<long> total_wins(Graph::size,0);
	int best_move=-1;
	int most_wins=0;
	int r;
	int row;
	int col;
	vector<int> open;
	vector<bool> checked(Graph::size, 0);
	random_device rd;
	pair<unsigned, unsigned> seed(static_cast<unsigned>(time(0)), rd()); //These are the seperate seeds for the rng, a different one for each thread
	
	//Run mc trials. This is a lambda function to be threaded.
	auto run_trials=[this](vector<int> open, long& wins,unsigned int seed)->void{
		thread_local minstd_rand generator(seed);
		
		for(int k=0;k<num_mc_trials/2;k++){
			if(!mc_trial(open,generator)){wins++;}
		}
	};
	
	open.reserve(Graph::size); //Eliminate costs from rescaling open
	
	//Go through every position in the graph to attempt a play and run mc trials from that position
	for(int i=0;i<Graph::size;i++){
		row=get_row(i);
		col=get_col(i);
		
		
		//Make a play on the tile. If it goes through, exectute the following code block.
		if(!play(!human_player, row ,col)){
			open.resize(0); //Clear the open vector
			
			//Fill the open vector with all the open tiles on the board
			for(int j=0;j<Graph::size;j++){
				if(hex_status.at(j)==Status::OPEN){open.push_back(j);}
			}
			
			//Run two threads with each taking half of the MC trials. Only difference is the seed for the rng
			wins1=0;
			wins2=0;
			thread th1(run_trials,open,ref(wins1),seed.first);
			thread th2(run_trials,open,ref(wins2),seed.second);
			th1.join();
			th2.join();
			
			//Combine the wins from each thread.
			total_wins.at(i)=wins1+wins2;
			
			//Take back the move
			hex_status.at(i)=Status::OPEN;
			
			//Update the best move
			if(total_wins.at(i)>most_wins){
				most_wins=total_wins.at(i);
				best_move=i;
			}
		}
	}
	
	row=get_row(best_move);
	col=get_col(best_move);
	r=play(!human_player, row, col);
	
	assert(("invalid move resulted from ai_turn",r==0)); //Make sure the ai returns a valid move.
	
	return iswinning(best_move, hex_status, checked);
}

//Fill in open positions randomly and return 1 if the trial results in a win for the human player
template <class UniformRandomBitGenerator>
bool Hex::mc_trial(vector<int> open, UniformRandomBitGenerator& rng){
	vector<Status> mc_status=hex_status; //Start with (*this)'s current board.
	int mc_turn=0;
	auto ai_begin=open.begin()+(open.size()/2.0+0.5); //Last turn was trial turn by the ai, so if there are an odd number of moves left, extra move goes to human
	vector<bool> checked(Graph::size, 0);
	
	shuffle(open.begin(),open.end(),rng);
	
	//Fill up the tiles with red or blue
	for(auto it=open.begin();it<ai_begin;it++){
		mc_status.at(*it)=static_cast<Status>(human_player);
	}
	for(auto it=ai_begin;it<open.end();it++){
		mc_status.at(*it)=static_cast<Status>(!human_player);
	}
		
	//Go down the left column and check if any blue moves are connected to the other side.
	//iswinning will update the checked vector so it wont be called on multiple connected moves
	for(int i=0;i<Graph::size;i+=size){
		if(mc_status.at(i)==Status::BLUE && !checked.at(i)){
			if(iswinning(i,mc_status, checked)){return human_player==Player::BLUE;} //BLUE won. So if human is BLUE, return 1, else return 0
		}
	}
	
	//If BLUE didn't win then RED must have won (known property of Hex games)
	//So if human is RED, return 1. Else return 0.
	return human_player==Player::RED;
}

//Contains the gameplay. Returns after the game; 1 if the human wins, 0 else.
bool Hex::start(){
	bool game_over;
	clock_t timer;
	double time_sec;
	
	display();
	
	cout<<"Enter B to play as BLUE, R to play as RED"<<endl;
	cin>>human_player;
	
	if(human_player==Player::BLUE){
		game_over=human_turn();
		display();
		
		if(game_over){return 1;}
	}
	
	while(1){
		game_over=0;
		
		cout<<"Thinking..."<<endl;
		
		timer=clock();
		
		game_over=ai_turn();
		
		timer=clock()-timer;
		
		display();
		
		time_sec=static_cast<double>(timer)/CLOCKS_PER_SEC;
		cout<<"The computer took "<<time_sec<<" s."<<endl;
		
		if(game_over){return 0;}
		
		game_over=human_turn();
		display();
		
		if(game_over){return 1;}
	}
}
