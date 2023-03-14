#pragma once
#pragma warning (disable: 4018) // fk signed/unsigned mismatch
#include <unordered_set>
#include "buttons.h"
#include "grids.h"
#include "Ship.h"
#include "gameBoard.h"
using namespace game_framework;

// Path: Source\Game\tinyUtil.h

int myIsOverlap(const CPoint& pt1, Ship* ship);
Ship* MakeAShip(const int& tp);
Ship* copyCatAShip(Ship* ship);// for test, not gonna be used in future
gameBoard copyCatABoard(const gameBoard& copied);
