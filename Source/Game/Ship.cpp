#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "tinyUtil.h"
#include "Ship.h"

int Ship::getHealth() const {
	return _health;
}

direction Ship::getDirection() {
	return this->GetFrameIndexOfBitmap() == 0 ? horizontal : vertical;
}

void Ship::damaged() {
	// this->SetFrameIndexOfBitmap(this->GetFrameIndexOfBitmap() + 2);
}

void Ship::sink() {
	// this->SetFrameIndexOfBitmap(this->GetFrameIndexOfBitmap() + 4);
}

void Ship::beingHit() {
	if (--_health == 0) {
		this->sink();
	} else {
		this->damaged();
	}
}

int Ship::getSize() {
	// {2, 3, 4, 5, 9} ->{ 2, 3, 3, 4, 5}
	if (_type < 4)
		return _type;
	if (_type < 6)
		return _type - 1;
	if (_type == 9)
		return 5;
	exit(-1);
}

void Ship::rotate() {
	this->SetFrameIndexOfBitmap((this->GetFrameIndexOfBitmap() + 1) % 2);
}

int GameBoard::getSelectedShipIndex() const {
	return _selectedShip;
}

vector<shared_ptr<Ship>> GameBoard::getShip() {
	return _ships;
}

shared_ptr<Ship> makeAShip(const int& tp) {
	shared_ptr<Ship> ship(new Ship);
	ship->_type = tp;
	ship->_health = ship->getSize();
	ship->_placeable = false;
	const string baseAddress = "Resources/ships/";
	vector<string> fileNames = {"headAtLeft.bmp", "headAtTop.bmp", "Resources/gridHit.bmp"};
	string folderName("A/");
	folderName[0] += tp - 2;// {2, 3, 4, 5, 9} ->{ 'A', 'B', 'C', 'D', 'H'}
	for (int i = 0; i < 2; ++i) {
		fileNames.at(i) = baseAddress + folderName + fileNames.at(i);
	}
	ship->LoadBitmapByString(fileNames);
	// ship->SetFrameIndexOfBitmap(1);
	return ship;
}

shared_ptr<Ship> copyAShip(shared_ptr<Ship> ship) {
	shared_ptr<Ship> newShip = (makeAShip(ship->_type));
	newShip->_shipId = ship->_shipId;
	newShip->SetFrameIndexOfBitmap(ship->GetFrameIndexOfBitmap());
	newShip->SetTopLeft(ship->GetLeft() + 1020, ship->GetTop());
	return newShip;

}
