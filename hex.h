//Jasper Hatton, 2020
//A class for a hex game which is derived from a graph class

#include "graph.h"
#include <string>
#include <limits>

enum class Status : short {BLUE=0,RED=1,OPEN=2}; //Describes the states of a tile

enum class Player : short {BLUE=0,RED=1};

enum class Side : short {TOP,BOTTOM,LEFT,RIGHT,NONE};

class Hex : public Graph<int>{
	protected:
		const int size;
		const long int num_mc_trials;
		std::vector<Status> hex_status;
		Player human_player=Player::BLUE;
		
		bool left_side(int x) const;
		bool right_side(int x) const;
		bool top(int x) const;
		bool bottom(int x) const;
		int get_col(int x) const;
		int get_row(int x) const;	
		
		bool human_turn();
		bool ai_turn();
				
		bool iswinning(int x, std::vector<Status> status_vec, std::vector<bool>& checked);
		
		template <class UniformRandomBitGenerator>
		bool mc_trial(std::vector<int> open, UniformRandomBitGenerator& rng);
		
		void display(std::vector<Status> hex_status);
	public:
		Hex(int size=11, long int trials=1000);
		~Hex();
		void display();	
		bool play(Player player, int& row, int& col,int x=0, int y=0);
		
		bool start();
};


Player operator!(Player player);
	
std::ostream& operator << (std::ostream& out, Status& x);
std::istream& operator >> (std::istream& in, Player& x);
std::istream& operator>>(std::istream& in, std::pair<int,int>& x);

template<class T>
void get_input(T& x, std::string ask_for_input);

template<class T>
void get_input(T& x, std::string ask_for_input){
	std::cout<<ask_for_input;
	while(!(std::cin>>x)){
		std::cout<<"Invalid character! Try again. \n \n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout<<ask_for_input;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<std::endl;
}