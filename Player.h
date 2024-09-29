#ifndef PLAYER
#define PLAYER
#include <iostream>
#include "Board.h"
#include "Side.h"

class Player {
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};

class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name);
	bool isInteractive() const;
	int chooseMove(const Board& b, Side s) const;

};

class BadPlayer : public Player {
public:
	BadPlayer(std::string name);
	int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player {
public:
	SmartPlayer(std::string name);
	int chooseMove(const Board& b, Side s) const;
private:
	void chooseMove(const Side& bot_side, Side& current_side, const Board& b, int& bestHole, int& value, int treeDepth, double& timeLimit, JumpyTimer& timer) const;
	int evaluate(const Board& b, const Side& s, const Side& opp_s) const;
};

#endif
