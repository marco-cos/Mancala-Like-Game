#include "Player.h"
#include "Game.h"
#include <stack>
#include <map>
#include <vector>
#include <random>

//Base class function implementations
Player::Player(std::string name) {
	m_name = name;
}

std::string Player::name() const {
	return m_name;
}

bool Player::isInteractive() const {
	return false;
}

Player::~Player() {}

//BadPlayer function implementations
BadPlayer::BadPlayer(std::string name) : Player(name) {}

int BadPlayer::chooseMove(const Board& b, Side s) const{
	std::vector<int> possibleMoves;
	for (int i = 1; i <= b.holes(); i++)
		if (b.beans(s, i) > 0)
			possibleMoves.push_back(i);

	if (possibleMoves.size() == 0)
		return -1; //No move is possible
	
	//Generate random move (random from hw7)
	std::random_device randomdevice;
	std::default_random_engine randomgenerator(randomdevice());
	std::uniform_int_distribution<> possible(0, possibleMoves.size()-1);

	return(possibleMoves.at(possible(randomgenerator)));
}

//HumanPlayer function implementations
HumanPlayer::HumanPlayer(std::string name) : Player(name){}

bool HumanPlayer::isInteractive() const {
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const {
	int move;
	bool over = false;

	while (!over) {

		std::cout << "Which hole would you like to move? " << std::endl;
		std::cin >> move;

		if ((move!=0)&&(b.beans(s, move) > 0))
			return move;
		else
			std::cout << "Invalid move, prompting again." << std::endl;

		//Check if there are still any valid moves before prompting again, else end move
		if ((b.beansInPlay(NORTH) == 0) || (b.beansInPlay(SOUTH) == 0))
			over = true;

	}

	return -1;
}

//Smartplayer function implementations
SmartPlayer::SmartPlayer(std::string name) : Player(name) {}

int  SmartPlayer::chooseMove(const Board& b, Side s) const {
	int bestHole;
	int value;
	
	double timelimit = 4500;
	JumpyTimer timer(1000);

	chooseMove(s, s, b, bestHole, value, 6,timelimit,timer);
	return bestHole;
}

int SmartPlayer::evaluate(const Board& b, const Side& s, const Side& opp_s) const {
	
	//If there are more beans in the pot than all in play and opponents pot, player will inevitably  or has already win
	if (b.beans(s, 0) > (b.beansInPlay(s) + b.beansInPlay(opp_s) + b.beans(opp_s,0)))
		return infinity;
	//If there are more beans in opponents pot than in play in the whole game and in player pot, then player will lose
	else if (b.beans(opp_s, 0) > (b.beansInPlay(s) + b.beansInPlay(opp_s) + b.beans(s, 0)))
		return -1*infinity;
	//If none of these coneditions are met, return difference in pots
	else
		return b.beans(s, 0) - b.beans(opp_s, 0);

}

void SmartPlayer::chooseMove(const Side& bot_side, Side& current_side, const Board& b, int& bestHole, int& value, int treeDepth,double& timeLimit, JumpyTimer& timer) const {
	//Get opponentside
	Side opponentside;
	current_side == NORTH ? opponentside = SOUTH : opponentside = NORTH;

	//If time is up, return
	if ((treeDepth % 2 == 0) && (timer.elapsed() > timeLimit)) {
		bestHole = -1;
		value = evaluate(b, current_side, opponentside);
		return;
	}
	else if (timer.elapsed() > timeLimit) {
		bestHole = -1;
		value = -76192;
		return;
	}

	//Gather all possible moves for player
	std::stack<int> possibleMoves;
	for (int i = 1; i <= b.holes(); i++)
		if (b.beans(current_side, i) > 0)
			possibleMoves.push(i);	


	//If game is over, see who won and return accordingly
	if (possibleMoves.empty()) {
		bestHole = -1;
		if (b.beans(current_side, 0) > b.beans(opponentside, 0))
			value = infinity; //Win for side
		else if (b.beans(current_side, 0) < b.beans(opponentside, 0))
			value = -1 * infinity; //Loss for side
		else
			value = 0; //Tie
		return;
	}
	//If treedepth is met, return
	if (treeDepth == 0) {
		bestHole = -1;
		value = evaluate(b, current_side, opponentside);
		return;
	}

	//Test other moves
	bestHole = possibleMoves.top(); 
	int endhole;
	Side endside;
	std::multimap<int, int> allreturned;


	while (!possibleMoves.empty()) {
		//Get possible move from stack
		int top = possibleMoves.top();
		int h2 = top;
		int v2 = value;
		possibleMoves.pop();

		//Make new copy of board to test every time
		Board b_copy = b;		

		//Move beans
		b_copy.sow(current_side, h2, endside, endhole);

		//Do capture if needed
		bool captured = false;
		if ((endside == current_side) && (b_copy.beans(current_side, endhole) == 1) && (b_copy.beans(opponentside, endhole) != 0) &&(endhole!=0)) {
			b_copy.moveToPot(current_side, endhole, current_side);
			b_copy.moveToPot(opponentside, endhole, current_side);
			captured = true;
		}

		//Sweep if needed
		if ((b_copy.beansInPlay(SOUTH) == 0 || b_copy.beansInPlay(NORTH) == 0) && (!captured)) {
			if (b.beansInPlay(SOUTH) == 0)
				for (int i = 1; i < b_copy.holes(); i++)
					b_copy.moveToPot(NORTH, i, NORTH);
			if (b.beansInPlay(NORTH) == 0)
				for (int i = 1; i < b_copy.holes(); i++)
					b_copy.moveToPot(SOUTH, i, SOUTH);
		}
		
		if ((endside == current_side) && (endhole == 0)) //Go again if lands in pot, don't adjust treedepth
			chooseMove(bot_side, current_side, b_copy, h2, v2, treeDepth,timeLimit, timer);
		else
			chooseMove(bot_side, opponentside, b_copy, h2, v2, treeDepth - 1, timeLimit, timer); //Otherwise, it's other players turn
		
		//If timer forced stop at odd depth, calculate evaluate here
		if (v2 == -76192) {
			bestHole = -1;
			v2 = evaluate(b, current_side, opponentside);
		}
	
		//Add values to map
		allreturned.insert(std::make_pair(v2, top));
	}

	//Get max and min of returned values
	std::multimap<int, int>::iterator max = allreturned.begin();
	std::multimap<int, int>::iterator min = allreturned.begin();

	for (std::multimap<int, int>::iterator it = allreturned.begin(); it != allreturned.end(); it++) {
		if (it->first < min->first)
			min = it;
		if (it->first > max->first)
			max = it;
	}

	//Wantmax determines which player you want to win/lose
	if (current_side == bot_side) {		
		bestHole = max->second;
		value = max->first;
	}
	else if (current_side != bot_side) {
		bestHole = min->second;
		value = min->first;
	}

}