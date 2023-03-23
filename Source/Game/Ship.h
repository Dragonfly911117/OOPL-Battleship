#pragma once

enum direction { horizontal, vertical };

class Ship : public BaseGrid {
	friend Ship* makeAShip(const int& tp);
	friend Ship* copyAShip(Ship* ship);
	friend int myIsOverlap(const CPoint& pt1, Ship* ship);
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
