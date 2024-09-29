#ifndef BOARD_HEADER
#define BOARD_HEADER
#include "Side.h"

//Board class definition
class Board {
public:
	Board(int nHoles, int nInitialBeansPerHole);
	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const; 
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
	Board(const Board& b);
	void operator=(const Board& b);
	~Board();
private:
	struct hole_list {
		Side m_side;
		int m_holenum;
		int m_beans;
		bool m_ispot;
		hole_list* next;
		hole_list* prev;
	};
	
	hole_list* m_northpot;
	hole_list* m_southpot;

	int m_holes;
	
	hole_list* getPointer(const Side s, const int hole) const;
};

#endif
