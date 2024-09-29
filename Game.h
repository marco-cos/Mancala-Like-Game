#ifndef GAME
#define GAME
#include "Board.h"
#include "Player.h"

class Game {
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move(Side s);
	void play();
	int beans(Side s, int hole) const;
private:
	Board board;
	Player* southplayer;
	Player* northplayer;
	bool gameover;
};

#endif