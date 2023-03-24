#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"

#include "mygame.h"

#include <assert.h>

void GameBoard::pickUpShip(const int& shipIndex) {
	_selectedShip = shipIndex;
	const int x = (_ships.at(shipIndex)->GetLeft() - _baseX) / 60;
	const int y = (_ships.at(shipIndex)->GetTop() - _baseX) / 60;
	const direction d = _ships.at(shipIndex)->getDirection();
	for (int i = 0; i < _ships.at(_selectedShip)->getSize(); ++i) {
		if (x < 10 && y < 10) {
			const auto grid = new EmptyGrid;
			grid->LoadBitmapA(R"(Resources/emptyGrid.bmp)");
			if (d == horizontal) {
				getGridByCoordinate(x + i, y)->shipPickingUpHere();
			} else if (d == vertical) {
				getGridByCoordinate(x, y + i)->shipPickingUpHere();
			} else {
				continue;
			}
		}
	}
}

BaseGrid* GameBoard::getGridByCoordinate(const int& x, const int& y) const {
	assert(x >= 0 && x < 10 && y >= 0 && y < 10);
	return _grids.at(x).at(y);
}

bool GameBoard::ifShipIsPlaceable(const int& x, const int& y, const direction& d) const {
	for (int i = 0; i < _ships.at(_selectedShip)->getSize(); ++i) {
		if (d == horizontal) {
			if (x + i >= 10 || !getGridByCoordinate(x + i, y)->ifPlaceable()) {
				return false;
			}
		} else if (d == vertical) {
			if (y + i >= 10 || !getGridByCoordinate(x, y + i)->ifPlaceable()) {
				return false;
			}
		}
	}
	return true;
}

bool GameBoard::dropShip(const CPoint& pt) {
	const int x = (pt.x - _baseX) / 60;
	const int y = (pt.y - _baseY) / 60;
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return false;
	const direction d = _ships.at(_selectedShip)->getDirection();
	const bool flag = ifShipIsPlaceable(x, y, d);
	if (flag) {
		const CPoint newPos(getGridByCoordinate(x,y)->GetLeft(), getGridByCoordinate(x,y)->GetTop());
		_ships.at(_selectedShip)->SetTopLeft(newPos.x, newPos.y);
		for (int i = 0; i < _ships.at(_selectedShip)->getSize(); ++i) {
			if (d == horizontal) {
				getGridByCoordinate(x + i, y)->shipDroppingHere(_selectedShip);
			} else if (d == vertical) {
				getGridByCoordinate(x, y + i)->shipDroppingHere(_selectedShip);
			} else {
			}
		}
		_selectedShip = -1;
		return true;
	} else {
		return false;
	}
}

bool GameBoard::ifAllShipPlaced() const {
	if (_selectedShip != -1)
		return false;
	for (auto& ship: _ships) {
		if (ship->GetLeft() >= _baseX + 10 * 60)
			return false;
	}
	return true;
}

void GameBoard::gettingStart() {

	// wah
	// for (int i = 0; i < ships.size(); ++i) {
	//     const direction d = ships.at(i)->GetFrameIndexOfBitmap();
	//     const int x = (ships.at(i)->GetLeft() - baseX) / 60;
	//     const int y = (ships.at(i)->GetTop() - baseY) / 60;
	//     for (int j = 0; j < ships.at(i)->getSize(); ++j) {
	//         if (direction == 1) {
	//             grids.at(x + j).at(y)->setShipID(i);
	//         }
	//         else {
	//             grids.at(x).at(y + j)->setShipID(i);
	//         }
	//
	//     }
	// }
}

void GameBoard::becomeEnemy() {
	_isEnemy = true;
}

int GameBoard::beingHit(const int& x, const int& y) {
	if (getGridByCoordinate(x, y)->GetFrameIndexOfBitmap() == getGridByCoordinate(x, y)->GetFrameSizeOfBitmap() - 1)
		return INT_MAX;
	getGridByCoordinate(x, y)->SetFrameIndexOfBitmap(getGridByCoordinate(x, y)->GetFrameSizeOfBitmap() - 1);
	if (getGridByCoordinate(x, y)->getShipID() != -1) {
		_ships.at(getGridByCoordinate(x, y)->getShipID())->beingHit();
		const auto temp = new BaseGrid;
		temp->LoadBitmapA("Resources/shipHit.bmp");
		temp->SetTopLeft(getGridByCoordinate(x, y)->GetLeft(), getGridByCoordinate(x, y)->GetTop());
		_shipHit.push_back(temp);
	}
	return getGridByCoordinate(x, y)->getShipID();
}

bool GameBoard::ifAllShipSunk() const {
	for (auto& i: _ships) {
		if (i->getHealth() != 0)
			return false;
	}
	return true;
}

void GameBoard::init() {
	_selectedShip = -1;
	_baseY = _baseX = 150;
	const vector<string> fileName = {R"(Resources/emptyGrid.bmp)", R"(Resources/gridHit.bmp)"};
	for (int i = 0; i < 10; ++i) {
		vector<BaseGrid*> curr(10);
		for (int j = 0; j < 10; ++j) {
			curr.at(j) = new EmptyGrid;
			curr.at(j)->LoadBitmapByString(fileName);
			curr.at(j)->SetTopLeft(_baseX + 60 * i, _baseY + 60 * j);
		}
		_grids.push_back(curr);
	}
	for (int i = 2; i < 6; ++i) {
		_ships.emplace_back(makeAShip(i));
		_ships.back()->SetTopLeft(_baseX + (60 * 10), _baseY + 60 * (i - 2));
	}
	_ships.emplace_back(makeAShip((9)));
	_ships.back()->SetTopLeft(_baseX + 60 * 10, _baseY + 240);
	// _baseY = _baseX = 150;
}

void GameBoard::show() {
	if (!_isEnemy) {
		for (auto& i: _ships) {
			i->ShowBitmap();
		}
	}
	for (auto& i: _grids) {
		for (auto& j: i) {
			if (j->ifDisplay())
				j->ShowBitmap();
		}
	}
	if (_selectedShip != -1 && !_isEnemy) {
		_ships.at(_selectedShip)->ShowBitmap();
	}
	for (const auto& i: _shipHit) {
		i->ShowBitmap();
	}
}

void GameBoard::setBaseX(const int& x) {
	_baseX = x;
}

int GameBoard::getBaseX() const {
	return _baseX;
}

void GameBoard::rotateShip() {
	if (_selectedShip == -1) {
		return;
	}
	_ships.at(_selectedShip)->rotate();
}
