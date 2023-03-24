#pragma once
#include <memory>

enum direction { horizontal, vertical };

class Ship : public BaseGrid {
	friend shared_ptr<Ship> makeAShip(const int& tp);
	friend shared_ptr<Ship> copyAShip(shared_ptr<Ship> ship);
	friend int myIsOverlap(const CPoint& pt1, shared_ptr<Ship> ship);
	int _type;
	int _health;
	void damaged();
	void sink();

public:
	// shared methods & variables
	int getSize();
	int getHealth() const;
	direction getDirection();

	// placement phase
	void rotate();

	// main-game phase
	void beingHit();
};
