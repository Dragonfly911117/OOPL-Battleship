#pragma once;

using namespace game_framework;

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

class EmptyGrid : public BaseGrid {
};
