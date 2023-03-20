#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"

#include "mygame.h"

void GameBoard::pickUpShip(const int& shipIndex) {
	currently_sel_ship_ = shipIndex;
	const int x = (ships_.at(shipIndex)->GetLeft() - base_x_) / 60;
	const int y = (ships_.at(shipIndex)->GetTop() - base_x_) / 60;
	const direction d = ships_.at(shipIndex)->getDirection();
	for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
		if (x < 10 && y < 10) {
			const auto grid = new EmptyGrid;
			grid->LoadBitmapA(R"(Resources/emptyGrid.bmp)");
			if (d == horizontal)
				grids_.at(x + i).at(y)->shipPickingUpHere();
			else if (d == vertical)
				grids_.at(x).at(y + i)->shipPickingUpHere();
			else
				continue;
		}
	}
}

bool GameBoard::ifAllShipIsPlaceable(const int& x, const int& y, const direction& d) const {
	for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
		if (d == horizontal) {
			if (x + i >= 10 || !grids_.at(x + i).at(y)->ifPlaceable()) {
				return false;
			}
		} else if (d == vertical) {
			if (y + i >= 10 || !grids_.at(x).at(y + i)->ifPlaceable()) {
				return false;
			}
		}
	}
	return true;
}

void GameBoard::dropShip(const CPoint& pt) {
	const int x = (pt.x - base_x_) / 60;
	const int y = (pt.y - base_y_) / 60;
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return;
	const direction d = ships_.at(currently_sel_ship_)->getDirection();
	const bool flag = ifAllShipIsPlaceable(x, y, d);
	if (flag) {
		ships_.at(currently_sel_ship_)->SetTopLeft(grids_.at(x).at(y)->GetLeft(), grids_.at(x).at(y)->GetTop());
		for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
			if (d == horizontal) {
				grids_.at(x + i).at(y)->shipDroppingHere(currently_sel_ship_);
			} else if (d == vertical) {
				grids_.at(x).at(y + i)->shipDroppingHere(currently_sel_ship_);
			} else {
			}
		}
		currently_sel_ship_ = -1;
	}// else: may have to do something    
}

bool GameBoard::ifAllShipPlaced() const {
	if (currently_sel_ship_ != -1)
		return false;
	for (auto& ship: ships_) {
		if (ship->GetLeft() >= base_x_ + 10 * 60)
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

bool GameBoard::beingHit(const int& x, const int& y) {
	if (grids_.at(x).at(y)->GetFrameIndexOfBitmap() == grids_.at(x).at(y)->GetFrameSizeOfBitmap() - 1)
		return false;
	grids_.at(x).at(y)->SetFrameIndexOfBitmap(grids_.at(x).at(y)->GetFrameSizeOfBitmap() - 1);
	if (grids_.at(x).at(y)->getShipID() != -1) {
		ships_.at(grids_.at(x).at(y)->getShipID())->beingHit();
		const auto temp = new BaseGrid;
		temp->LoadBitmapA("Resources/shipHit.bmp");
		temp->SetTopLeft(grids_.at(x).at(y)->GetLeft(), grids_.at(x).at(y)->GetTop());
		ship_hit_.push_back(temp);
	}
	return true;
}

bool GameBoard::ifAllShipSunk() const {
	for (auto& i: ships_) {
		if (i->getHealth() != 0)
			return false;
	}
	return true;
}

void GameBoard::init() {
	currently_sel_ship_ = -1;
	base_y_ = base_x_ = 150;
	const vector<string> fileName = {R"(Resources/emptyGrid.bmp)", R"(Resources/gridHit.bmp)"};
	for (int i = 0; i < 10; ++i) {
		vector<BaseGrid*> curr(10);
		for (int j = 0; j < 10; ++j) {
			curr.at(j) = new EmptyGrid;
			curr.at(j)->LoadBitmapByString(fileName);
			curr.at(j)->SetTopLeft(base_x_ + 60 * i, 60 * j + base_y_);
		}
		grids_.push_back(curr);
	}
	for (int i = 2; i < 6; ++i) {
		ships_.emplace_back(makeAShip(i));
		ships_.back()->SetTopLeft(base_x_ + (60 * 10), (i - 2) * 60 + base_y_);
	}
	ships_.emplace_back(makeAShip((9)));
	ships_.back()->SetTopLeft(base_x_ + 60 * 10, base_y_ + 240);
	// base_y_ = base_x_ = 150;
}

void GameBoard::show() {
	if (! is_enemy_)
		for (auto& i: ships_) {
			i->ShowBitmap();
		}
	for (auto& i: grids_) {
		for (auto& j: i) {
			if (j->ifDisplay())
				j->ShowBitmap();
		}
	}
	if (currently_sel_ship_ != -1 && !is_enemy_)
		ships_.at(currently_sel_ship_)->ShowBitmap();
	for (const auto& i: ship_hit_)
		i->ShowBitmap();
}

void GameBoard::rotateShip() {
	if (currently_sel_ship_ == -1)
		return;
	ships_.at(currently_sel_ship_)->rotate();
}
