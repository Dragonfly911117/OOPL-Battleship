#pragma once
using namespace game_framework;

class BaseGrid;
class Ship;
enum direction;

class GameBoard {
	friend GameBoard copyABoard(const GameBoard& copied);
	friend GameBoard generateABoard(const int& x);
	vector<vector<BaseGrid*>> _grids;
	vector<Ship*> _ships;       // for all ships
	vector<BaseGrid*> _shipHit;// for all grids that have ship on it and are hit, this is for displaying the hit effect
	int _selectedShip = -1;
	int _baseX = 150;
	int _baseY = 150;
	bool _isEnemy = false;
	BaseGrid* getGridByCoordinate(const int& x, const int& y) const ;
	bool ifShipIsPlaceable(const int& x, const int& y, const direction& d) const;

public:
	// shared methods & variables
	vector<Ship*> getShip();
	void show();
	void setBaseX(const int& x);
	int getBaseX() const;

	// deployment-phase methods
	void init();
	int getSelectedShipIndex() const;
	void pickUpShip(const int& shipIndex);
	void rotateShip();
	bool dropShip(const CPoint& pt);
	bool ifAllShipPlaced() const;
	void gettingStart();

	// main-game-phase methods
	void becomeEnemy();
	int beingHit(const int& x, const int& y);
	bool ifAllShipSunk() const;
};
