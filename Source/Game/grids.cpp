#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "grids.h"

bool BaseGrid::getDisplayFlag() {
	return _displayFlag;
}

void BaseGrid::setDisplayFlag(const bool& flag) {
	_displayFlag = flag;
}

void BaseGrid::shipDroppingHere(const int& id) {
	_placeable = _displayFlag = false;
	_shipId = id;
}

void BaseGrid::pickUpShip() {
	_placeable = _displayFlag = true;
	_shipId = -1;
}

bool BaseGrid::ifPlaceable() {
	return _placeable;
}

CPoint BaseGrid::getCoordinate() {
	return CPoint(this->GetLeft(), this->GetTop());
}

int BaseGrid::getShipId() {
	return _shipId;
}

void BaseGrid::setShipId(const int& id) {
	_shipId = id;
}
