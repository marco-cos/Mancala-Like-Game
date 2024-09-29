#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north) : board(b){
	//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
	southplayer = south;
	northplayer = north;
	gameover = false;

}

void Game::display() const {
	//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
	std::cout << std::endl<<  "   " << northplayer->name() << "'s Side"<<std::endl << "   ";

	for (int i = 1; i <= board.holes(); i++)  //Display all north side
		std::cout << "|" << beans(NORTH, i) << "|";

	std::cout << std::endl << "|" << beans(NORTH, 0) << "|"; //Display north pot

	for (int i = 0; i < board.holes(); i++)  //Make proper amount of spaces
		std::cout << "   ";
	std::cout << "|" << beans(SOUTH, 0) << "|" << std::endl << "   "; //Display south pot

	for (int i = 1; i <= board.holes(); i++)  //Display all south side
		std::cout << "|" << beans(SOUTH, i) << "|";

	std::cout << std::endl << "   " << southplayer->name() << "'s Side" << std::endl << std::endl << std::endl;
	
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
	//If the game is over (i.e., the move member function has been called and returned false), 
	//set over to true; otherwise, set over to false and do not change anything else. 
	//If the game is over, set hasWinner to true if the game has a winner, or false if it resulted 
	//in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
	if (board.beansInPlay(SOUTH) + board.beansInPlay(NORTH) == 0) {
		over = true;
		if (board.beans(NORTH, 0) > board.beans(SOUTH, 0)) { //NORTH wins
			hasWinner = true;
			winner = NORTH;
		}
		else if (board.beans(NORTH, 0) < board.beans(SOUTH, 0)) { //SOUTH wins
			hasWinner = true;
			winner = SOUTH;
		}
		else { //Tie
			hasWinner = false;
		}
	}
	else {
		over = false;
	}
}

bool Game::move(Side s) {
	//Attempt to make a complete move for the player playing side s. "Complete" means that the player 
	//sows the seeds from a hole and takes any additional turns required or completes a capture. 
	//Whenever the player gets an additional turn, you should display the board so someone looking 
	//at the screen can follow what's happening. If the move can be completed, return true; if not, 
	//because the move is not yet completed but side s has no holes with beans to pick up and sow, 
	//sweep any beans in s's opponent's holes into that opponent's pot and return false.
	int playermove;
	Side opponentside;
	std::string playername;
	
	//Check if game is already over
	if (gameover)
		return false;

	if (s == NORTH) {
		playermove = northplayer->chooseMove(board, NORTH);

		opponentside = SOUTH;
		playername = northplayer->name();
	}
	else {
		playermove = southplayer->chooseMove(board, SOUTH);
		opponentside = NORTH;
		playername = southplayer->name();
	}

	if (playermove == -1) {
		for (int i = 1; i <= board.holes();i++)
			board.moveToPot(opponentside, i, opponentside); //Sweep everything on opponents side to their pot
		gameover = true;
		return false; //No move possible
	}

	Side endside;
	int endhole;
	board.sow(s, playermove, endside, endhole);

	//Code for capture
	if ((endside == s) && (board.beans(s, endhole) == 1) && (board.beans(opponentside, endhole) != 0)&&(endhole!=0)) {
		std::cout << playername << " has played " << playermove << " and performed a capture!" << std::endl;
		board.moveToPot(s, endhole, s);
		board.moveToPot(opponentside, endhole, s);

		//Other player gets to go again even if capture results in player side being an empty board
		if ((board.beansInPlay(s) == 0)&&(board.beansInPlay(opponentside)!=0)) {
			move(opponentside);
			return true;
		}
	}
	else {
		std::cout << playername << " has played hole " << playermove <<":"; //Normal case cout
	}
	display(); //Display new board

	//Check if game is now over
	if (board.beansInPlay(s)==0)
		for (int i = 1; i <= board.holes(); i++)
			board.moveToPot(opponentside, i, opponentside); //Sweep everything on opponents side to their pot
	else if (board.beansInPlay(opponentside)==0)
		for (int i = 1; i <= board.holes(); i++)
			board.moveToPot(s, i, s); //Sweep everything on player side to their pot
	
	//Player goes again if they end in their pot and have beans available to play
	if ((endside == s) && (endhole == 0) && (board.beansInPlay(s)!=0)) {
		std::cout << playername << " gets to go again " << std::endl;
		return move(s);
	}

	return true;
}

void Game::play() {
	//Play the game.Display the progress of the game in a manner of your choosing, provided that someone
	//looking at the screen can follow what's happening. If neither player is interactive, then to keep 
	//the display from quickly scrolling through the whole game, it would be reasonable periodically to 
	//prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. 
	//(The ignore function for input streams is useful here.) Announce the winner at the end of the game. 
	//You can apportion to your liking the responsibility for displaying the board between this function and the move function.
	Side player = SOUTH;
	display();

	while (true) {
		move(player);

		//Announce winners if game is now over
		if ((board.beansInPlay(NORTH) == 0) || (board.beansInPlay(SOUTH) == 0)) {
			if ((board.beans(NORTH,0)) > (board.beans(SOUTH, 0)))
				std::cout << northplayer->name() << " has won!";
			else if ((board.beans(NORTH, 0)) < (board.beans(SOUTH, 0)))
				std::cout << southplayer->name() << " has won!";
			else
				std::cout << "It's a tie!";
			display();
			return;
		}

		//If game isn't over, switch to next player
		if (player == SOUTH)
			player = NORTH;
		else
			player = SOUTH;
	}
}

int Game::beans(Side s, int hole) const {
	//Return the number of beans in the indicated hole or pot of the game's board, 
	//or −1 if the hole number is invalid. This function exists so that we and you can more easily 
	//test your program.
	return board.beans(s, hole);
}
