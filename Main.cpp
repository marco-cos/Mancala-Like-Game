#include <iostream>
#include "Board.h"
#include "Game.h"
#include "Player.h"

int main() {
	HumanPlayer human("Human");
	SmartPlayer bot("Bot");
	Board b(6, 4);
	Game g(b, &human, &bot);
	g.play();
}