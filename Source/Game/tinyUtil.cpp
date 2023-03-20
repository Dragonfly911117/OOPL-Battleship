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

// return the block index if overlap, return 0 if not overlap, return -1 if overlap but not on a block.
int myIsOverlap(const CPoint& pt1, Ship* ship) {
	auto lt = CPoint(ship->GetLeft(), ship->GetTop());
	CPoint rb;// dont use GetHeight()  and GetWidth(). They are not the same as the size of the bitmap after rotations.
	int direction = ship->GetFrameIndexOfBitmap();
	rb = direction == 0
		     ? CPoint(lt.x + ship->getSize() * 60 - 10, lt.y + 50)
		     : CPoint(lt.x + 50, lt.y + ship->getSize() * 60 - 10);
	if (pt1.x > lt.x && pt1.x < rb.x && pt1.y > lt.y && pt1.y < rb.y) {
		// which block exactly is clicked
		for (int i = 1; i <= ship->getSize(); ++i) {
			CPoint pt2 = direction == 0
				             ? CPoint(lt.x + i * 60 - 10, lt.y + 50)
				             : CPoint(lt.x + 50, lt.y + i * 60 - 10);
			if (pt1.x > pt2.x - 50 && pt1.x < pt2.x && pt1.y > pt2.y - 50 && pt1.y < pt2.y) { return i; }
		}
		return -1;
	}
	return 0;
}

GameBoard copyABoard(const GameBoard& copied) {
	GameBoard newBoard;
	newBoard.base_x_ = copied.base_x_;
	newBoard.base_y_ = copied.base_y_;
	newBoard.is_enemy_ = true;
	vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};
	for (int i = 0; i < 10; ++i) {
		vector<BaseGrid*> curr;
		for (int j = 0; j < 10; ++j) {
			auto newGrid = new EmptyGrid;
			newGrid->LoadBitmapByString(fileName);
			newGrid->setShipID(copied.grids_.at(i).at(j)->getShipID());
			newGrid->SetTopLeft(copied.grids_.at(i).at(j)->GetLeft() + 1020, copied.grids_.at(i).at(j)->GetTop());
			curr.push_back(newGrid);
		}
		newBoard.grids_.push_back(curr);
	}
	for (auto& ship: copied.ships_) { newBoard.ships_.push_back(copyAShip(ship)); }
	return newBoard;
}
