#pragma once
#include "grids.h"

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
