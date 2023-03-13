#pragma once
#include <unordered_map>
#include <unordered_set>
using namespace game_framework;
#pragma warning (disable: 4018) // fk signed/unsigned mismatch 
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
    friend Ship* stealAShip(Ship* ship);
    friend int myIsOverlap(const CPoint& pt1, Ship* ship);
    int int_type_;
    int int_health_;
    bool displayFlag = true;
    void damaged();
    void sink();

public:
    // shared methods & variables
    int getSize();

    // placement phase
    void rotate();

    // main-game phase
    void beingHit();
};

int myIsOverlap(const CPoint& pt1, Ship* ship);
Ship* MakeAShip(const int& tp);
Ship* stealAShip(Ship* ship);// for test, not gonna be used in future
class EmptyGrid : public BaseGrid {};

class gameBoard : public CMovingBitmap {
    // Since setup pos before  CGameStateRun::OnInit() crashes the game Constructors are NOT used 
    vector<vector<BaseGrid*>> grids;
    vector<Ship*> ships;
    int currentlySelShip = -1;
    int baseX, baseY;

public:
    // shared methods & variables
    vector<Ship*> getShip();
    void show();

    // deployment-phase methods
    void init();
    int getCurrSel();
    void pickUpShip(const int& sel);
    void rotateShip();
    void dropShip(const CPoint& pt);
    bool ifAllShipPlaced();
    void gettingStart();
    void whatEasesMyPainCannotBeCalledSteal(const gameBoard& copied);

    // main-game-phase methods
    bool beingHit(const int& x, const int& y);
};
