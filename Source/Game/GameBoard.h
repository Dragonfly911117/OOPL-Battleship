#pragma once
using namespace game_framework;

class BaseGrid;
class Ship;

class GameBoard {
	friend GameBoard copyABoard(const GameBoard& copied);
	// Since setup pos before  CGameStateRun::OnInit() crashes the game Constructors are NOT used 
	vector<vector<BaseGrid*>> grids_;
	vector<Ship*> ships_;       // for all ships
	vector<BaseGrid*> ship_hit_;// for all grids that have ship on it and are hit, this is for displaying the hit effect
	int currently_sel_ship_ = -1;
	int base_x_ = 0;
	int base_y_ = 0;
	bool is_enemy_ = false;
	bool ifAllShipIsPlaceable(const int& x, const int& y, const direction& d) const;

public:
	// shared methods & variables
	vector<Ship*> getShip();
	void show();

	// deployment-phase methods
	void init();
	int getCurrSel() const;
	void pickUpShip(const int& shipIndex);
	void rotateShip();
	void dropShip(const CPoint& pt);
	bool ifAllShipPlaced() const;
	void gettingStart();

	// main-game-phase methods
	bool beingHit(const int& x, const int& y);
	bool ifAllShipSunk() const;
};
