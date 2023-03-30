#pragma once
#include <memory>
using namespace game_framework;

class BaseGrid;
class Ship;
enum direction;

class GameBoard {
	friend GameBoard copyABoard(const GameBoard& copied);
	friend GameBoard generateABoard(const int& x, const bool& isEnemy);
	vector<vector<shared_ptr<BaseGrid>>> _grids;
	vector<shared_ptr<Ship>> _ships;      // for all ships
	vector<shared_ptr<BaseGrid>> _shipHit;// for all grids that have ship on it and are hit, this is for displaying the hit effect
	int _selectedShip = -1;
	int _baseX = 150;
	int _baseY = 150;
	bool _isEnemy = false;
	BaseGrid* getGridByCoordinate(const int& x, const int& y) const;
	bool ifShipIsPlaceable(const int& x, const int& y, const direction& d) const;

public:
	GameBoard& operator=(const GameBoard& copied);
	// shared methods & variables
	vector<shared_ptr<Ship>> getShip();
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
