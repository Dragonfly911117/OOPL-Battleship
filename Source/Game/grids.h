#pragma once;

using namespace game_framework;

class BaseGrid : public CMovingBitmap {
protected:
	int _shipId = -1;
	bool _placeable = true;
	bool _displayFlag = true;

public:
	void shipDroppingHere(const int& id);
	void pickUpShip();
	bool ifPlaceable();
	CPoint getCoordinate();
	int getShipId();
	void setShipId(const int& id);
	bool getDisplayFlag();
	void setDisplayFlag(const bool& flag);
};

class EmptyGrid : public BaseGrid {
};
