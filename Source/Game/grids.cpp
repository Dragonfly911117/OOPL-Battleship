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

bool BaseGrid::ifDisplay() {
	return _displayFlag;
}


void BaseGrid::shipDroppingHere(const int& id) {
	_placeable = _displayFlag = false;
	_shipId = id;
}

void BaseGrid::shipPickingUpHere() {
	_placeable = _displayFlag = true;
	_shipId = -1;
}

bool BaseGrid::ifPlaceable() {
	return _placeable;
}

CPoint BaseGrid::getCoordinate() {
	return CPoint(this->GetLeft(), this->GetTop());
}

int BaseGrid::getShipID() {
	return _shipId;
}

void BaseGrid::setShipID(const int& id) {
	_shipId = id;
}
