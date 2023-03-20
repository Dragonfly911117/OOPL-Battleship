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

bool BaseGrid::ifDisplay() { return displayFlag; }

void BaseGrid::shipDroppingHere(const int& id) {
	placeable = displayFlag = false;
	shipID = id;
}

void BaseGrid::shipPickingUpHere() {
	placeable = displayFlag = true;
	shipID = -1;
}

bool BaseGrid::ifPlaceable() { return placeable; }

CPoint BaseGrid::getCoordinate() { return CPoint(this->GetLeft(), this->GetTop()); }

int BaseGrid::getShipID() { return shipID; }

void BaseGrid::setShipID(const int& id) { shipID = id; }
