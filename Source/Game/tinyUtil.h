#pragma once
#pragma warning (disable: 4018) // fk signed/unsigned mismatch
#include <unordered_set>
#include "buttons.h"
#include "grids.h"
#include "Ship.h"
#include "GameBoard.h"
#include "phaseManager.h"
using namespace game_framework;

int myIsOverlap(const CPoint& pt1, Ship* ship);
Ship* makeAShip(const int& tp);

Ship* copyAShip(Ship* ship);
GameBoard copyABoard(const GameBoard& copied);
// functions that are used to copy stuff is not gonna be used in future

void setupBackground();
