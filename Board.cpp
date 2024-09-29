#include "Board.h"
#include <iostream>


Board::Board(int nHoles, int nInitialBeansPerHole) {
	//Construct a Board with the indicated number of holes per side(not counting the pot)
	//and initial number of beans per hole.If nHoles is not positive, act as if it were 1; 
	//if nInitialBeansPerHole is negative, act as if it were 0.

	//Initialize holes and initialbeans member variables
	if (nHoles < 0)
		m_holes = 1;
	else
		m_holes = nHoles;

	int m_initialbeansperhole;
	if (nInitialBeansPerHole < 0)
		m_initialbeansperhole = 0;
	else
		m_initialbeansperhole = nInitialBeansPerHole;

	//Initialize linked list
	m_northpot = new hole_list;
	hole_list* current = m_northpot;
	current->m_holenum = 0;
	current->m_beans = 0;
	current->m_ispot = true;
	current->m_side = NORTH;


	//Make all nodes for north pot
	for (int i = 1; i != m_holes + 1; i++) {
		current->next = new hole_list;
		current->next->prev = current;
		current = current->next;

		current->m_holenum = i;
		current->m_beans = m_initialbeansperhole;
		current->m_ispot = false;
		current->m_side = NORTH;
	}


	//Make south pot
	current->next = new hole_list;
	current->next->prev = current;
	current = current->next;
	m_southpot = current;

	current->m_holenum = 0;
	current->m_beans = 0;
	current->m_ispot = true;
	current->m_side = SOUTH;

	//Make all nodes for south pot
	for (int i = m_holes; i != 0; i--) {
		current->next = new hole_list;
		current->next->prev = current;
		current = current->next;

		current->m_holenum = i;
		current->m_beans = m_initialbeansperhole;
		current->m_ispot = false;
		current->m_side = SOUTH;
	}

	//Make list circular
	m_northpot->prev = current;
	current->next = m_northpot;
}

int Board::holes() const {
	//Return the number of holes on a side(not counting the pot).
	return m_holes;
}
int Board::beans(Side s, int hole) const {
	//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
	//int* returned = gethole(s, hole);
	hole_list* it = getPointer(s, hole);

	if (it == nullptr)
		return -1; //Hole wasn't found
	else
		return it->m_beans;
}
int Board::beansInPlay(Side s) const {
	//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
	int total = 0;
	for (int i = 1; i <= m_holes; i++) //Add up all beans on passed in side
		total += beans(s, i);

	return total;
}
int Board::totalBeans() const {
	//Return the total number of beans in the game, including any in the pots.
	return beansInPlay(SOUTH)+beansInPlay(NORTH)+m_northpot->m_beans+m_southpot->m_beans;
}

Board::hole_list* Board::getPointer(const Side s, const int hole) const {

	if (hole > m_holes)
		return nullptr;

	//Get pointer to hole
	hole_list* it = m_northpot;
	if (s == NORTH)
		it = m_northpot;
	if (s == SOUTH)
		it = m_southpot;

	for (int i = 1; i <= m_holes; i++) {
		if (it->m_holenum == hole) //Hole matches case
			; //Doesn't need statement, just needs else to not execute
		else //Iterate if hole doesn't match
			it = it->next;
	}

	if (it->m_holenum != hole) //In case hole is not present
		return nullptr;

	return it;
}


bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
//	If the hole indicated by(s, hole) is empty or invalid or a pot, 
//	this function returns false without changing anything.Otherwise, 
//	it will return true after sowing the beans : the beans are removed from hole(s, hole) 
//	and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. 
//	The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. 
//	(This function does not make captures or multiple turns; different Kalah variants have 
//	different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
	hole_list* it = getPointer(s, hole);

	if ((hole==0)||(it==nullptr)||(beans(s,hole)==0)) //Check hole is not pot or invalid
		return false;

	//Empty hole
	int beans = it->m_beans;
	it->m_beans = 0;

	//Distribute beans
	while (beans != 0) {
		it = it->prev;

		//Checking it is not opposing pot
		if ((it->m_holenum == 0) && (s != it->m_side))
			it = it->prev;
		
		//Distrbute beans 
		it->m_beans += 1;
		beans--;
	}

	//Setting endside and endhole
	endHole = it->m_holenum;
	endSide = it->m_side;

	return true;
}
bool Board::moveToPot(Side s, int hole, Side potOwner) {
	//If the indicated hole is invalid or a pot, return false without changing anything.
	//Otherwise, move all the beans in hole(s, hole) into the pot belonging to potOwnerand return true.
	hole_list* it = getPointer(s, hole);

	if ((hole == 0) || (it == nullptr)) //Check hole is not pot or invalid
		return false;

	//Move the beans
	int beans = it->m_beans;
	it->m_beans = 0;
	if (potOwner == NORTH)
		m_northpot->m_beans += beans;
	if (potOwner == SOUTH)
		m_southpot->m_beans += beans;

	return true;
}
bool Board::setBeans(Side s, int hole, int beans) {
	//If the indicated hole is invalid or beans is negative, this function returns false without changing anything.
	//Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. 
	//(This could change what beansInPlay and totalBeans return if they are called later.) 
	//This function exists solely so that weand you can more easily test your program : 
	//None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. 
	//(We'll show an example of its use below.)
	if (beans < 0)
		return false;

	//Get pointer to hole
	hole_list* it = getPointer(s, hole);

	if (it == nullptr)
		return false;

	it->m_beans = beans;
	return true;
}

Board::Board(const Board& b) {
	m_holes = b.m_holes;

	//Make northpot
	m_northpot = new hole_list;
	hole_list* current = m_northpot;
	hole_list* copy = b.m_northpot->prev;

	for (int i = 0; i < (m_holes * 2) + 2; i++) {
		copy = copy->next;

		current->m_side = copy->m_side;
		current->m_holenum = copy->m_holenum;
		current->m_beans = copy->m_beans;
		current->m_ispot = copy->m_ispot;
		
		if ((copy->m_ispot) && (copy->m_side == SOUTH))
			m_southpot = current;

		current->next = new hole_list;
		current->next->prev = current;
		current = current->next;
	}

	//Delete last node, make list circularly linked
	current = current->prev; 
	delete current->next;
	current->next = m_northpot;
	m_northpot->prev = current;

}

void Board::operator=(const Board& b) {
	//Delete everything, then reuse copy constructor
	hole_list* it = m_northpot;
	for (int i = 1; i < (m_holes * 2) + 2; i++) {
		it = it->next;
		delete it->prev;
	}
	delete it;

	//Copy constructor code
	m_holes = b.m_holes;

	//Make northpot
	m_northpot = new hole_list;
	hole_list* current = m_northpot;
	hole_list* copy = b.m_northpot->prev;

	for (int i = 0; i < (m_holes * 2) + 2; i++) {
		copy = copy->next;

		current->m_side = copy->m_side;
		current->m_holenum = copy->m_holenum;
		current->m_beans = copy->m_beans;
		current->m_ispot = copy->m_ispot;

		if ((copy->m_ispot) && (copy->m_side == SOUTH))
			m_southpot = current;

		current->next = new hole_list;
		current->next->prev = current;
		current = current->next;
	}

	//Delete last node, make list circularly linked
	current = current->prev;
	delete current->next;
	current->next = m_northpot;
	m_northpot->prev = current;
}

Board::~Board() {
	hole_list* it = m_northpot;
	for (int i = 1; i < (m_holes * 2) + 2; i++) {
		it = it->next;
		delete it->prev;
	}
	delete it;
}