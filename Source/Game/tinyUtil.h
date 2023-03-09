﻿#pragma once
using namespace game_framework;

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
    bool placeable = true;
public:
    void invertPlaceableState();
    bool ifPlaceable();
};

class Ship;
bool myIsOverlap(const CPoint& pt1,  Ship* ship);
Ship* MakeAShip(const int& tp);

class Ship : public BaseGrid {
    friend Ship* MakeAShip(const int& tp);
    friend bool myIsOverlap(const CPoint& pt1, Ship* ship);
    int int_type_;
    int int_health_;

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

class EmptyGrid : public BaseGrid {
};

class gameBoard : public CMovingBitmap {
    // Since setup pos before  CGameStateRun::OnInit() crashes the game Constructors are NOT used 
    vector<vector<BaseGrid*>> grids;
    vector<Ship*> ships;
    int currentlySelShip = -1;
    int baseX, baseY;
public:
    int getCurrSel();
    vector<Ship*> getShip();
    void pickUpShip(const int& sel);
    void gameBoard::dropShip(const CPoint& pt);
    void init();
    void show();
    void rotateShip();
};
