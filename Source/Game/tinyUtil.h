#pragma once
#include <unordered_map>
#include <unordered_set>
using namespace game_framework;
#pragma warning (disable: 4018) // fk signed/unsigned mismatch

// Path: Source\Game\tinyUtil.h
class myBtn : public CMovingBitmap {
protected:
	string text;

public:
	myBtn();
	void setText(const string& str);
	void pressed();
	void released();
	void showBtn();
};

class BaseGrid : public CMovingBitmap {
protected:
	int shipID = -1;
	bool placeable = true;
	bool displayFlag = true;

public:
	void shipDroppingHere(const int& id);
	void shipPickingUpHere();
	bool ifPlaceable();
	CPoint getCoordinate();
	int getShipID();
	void setShipID(const int& id);
	bool ifDisplay();
};

class Ship : public BaseGrid {
	friend Ship* MakeAShip(const int& tp);
	friend Ship* copyCatAShip(Ship* ship);
	friend int myIsOverlap(const CPoint& pt1, Ship* ship);
	int int_type_;
	int int_health_;
	void damaged();
	void sink();

public:
	// shared methods & variables
	int getSize();
	int getHealth() const;

	// placement phase
	void rotate();

	// main-game phase
	void beingHit();
};

class EmptyGrid : public BaseGrid {
};

class gameBoard : public CMovingBitmap {
	friend gameBoard copyCatABoard(const gameBoard& copied);
	// Since setup pos before  CGameStateRun::OnInit() crashes the game Constructors are NOT used 
	vector<vector<BaseGrid*>> grids_;
	vector<Ship*> ships_;
	vector<BaseGrid*> ship_hit_;
	int currently_sel_ship_ = -1;
	int base_x_ = 0;
	int base_y_ = 0;
	bool is_enemy_ = false;

public:
	// shared methods & variables
	vector<Ship*> getShip();
	void show();

	// deployment-phase methods
	void init();
	int getCurrSel() const;
	void pickUpShip(const int& sel);
	void rotateShip();
	void dropShip(const CPoint& pt);
	bool ifAllShipPlaced() const;
	void gettingStart();
	void whatEasesMyPainCannotBeCalledSteal(const gameBoard& copied);

	// main-game-phase methods
	bool beingHit(const int& x, const int& y);
	bool ifAllShipSunk() const;
};

int myIsOverlap(const CPoint& pt1, Ship* ship);
Ship* MakeAShip(const int& tp);
Ship* copyCatAShip(Ship* ship);// for test, not gonna be used in future
gameBoard copyCatABoard(const gameBoard& copied);
