#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "tinyUtil.h"

#include <random>

// return the block index if overlap, return 0 if not overlap, return -1 if overlap but not on a block.
int myIsOverlap(const CPoint& pt1, Ship& ship) {
	const auto lt = CPoint(ship.GetLeft(), ship.GetTop());
	const direction d = ship.getDirection();
	const CPoint rb = d == horizontal
		                  ? CPoint(lt.x + ship.getSize() * 60 - 10, lt.y + 50)
		                  : CPoint(lt.x + 50, lt.y + ship.getSize() * 60 - 10);
	if (pt1.x > lt.x && pt1.x < rb.x && pt1.y > lt.y && pt1.y < rb.y) {
		// which block exactly is clicked
		for (int i = 1; i <= ship.getSize(); ++i) {
			const CPoint pt2 = d == horizontal
				                   ? CPoint(lt.x + 60 * i - 10, lt.y + 50)
				                   : CPoint(lt.x + 50, lt.y + 60 * i - 10);
			if (pt1.x > pt2.x - 50 && pt1.x < pt2.x && pt1.y > pt2.y - 50 && pt1.y < pt2.y) {
				return i;
			}
		}
		return -1;
	}
	return 0;
}

GameBoard generateABoard(const int& x, const bool& isEnemy) {
	GameBoard result;
	result._isEnemy = isEnemy;
	const vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};
	result._baseX = x;
	for (int i = 0; i < 10; ++i) {
		vector<shared_ptr<BaseGrid>> curr(10);
		for (int j = 0; j < 10; ++j) {
			curr.at(j).reset(new BaseGrid);
			curr.at(j)->LoadBitmapByString(fileName);
			curr.at(j)->SetTopLeft(result._baseX + 60 * i, result._baseY + 60 * j);
		}
		result._grids.push_back(curr);
	}
	for (int i = 2; i < 6; ++i) {
		result._ships.emplace_back(makeAShip(i));
	}
	result._ships.emplace_back(makeAShip((9)));

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> unif(0.0, 590.0);

	for (int i = 0; i < result._ships.size(); ++i) {
		if (static_cast<int>(unif(generator)) % 2) {
			result._ships.at(i)->rotate();
		}
		result._selectedShip = i;
		CPoint temp;
		temp.x = result._baseX + static_cast<int>(unif(generator));
		temp.y = result._baseY + static_cast<int>(unif(generator));
		while (! result.dropShip(temp)) {
			temp.x = result._baseX + static_cast<int>(unif(generator));
			temp.y = result._baseY + static_cast<int>(unif(generator));
		}
	}
	return result;
}

GameBoard copyABoard(const GameBoard& copied) {
	GameBoard newBoard;
	newBoard._baseX = 1020;
	newBoard._baseY = copied._baseY;
	newBoard._isEnemy = true;

	const vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};

	for (int i = 0; i < 10; ++i) {
		vector<shared_ptr<BaseGrid>> curr(10);
		for (int j = 0; j < 10; ++j) {
			curr.at(j).reset(new BaseGrid);
			curr.at(j)->LoadBitmapByString(fileName);
			curr.at(j)->SetTopLeft(copied.getGridByCoordinate(i, j)->GetLeft() + newBoard._baseX, copied.getGridByCoordinate(i, j)->GetTop());

		}
		newBoard._grids.emplace_back(curr);
	}

	for (auto& ship: copied._ships) {
		newBoard._ships.emplace_back(copyAShip(ship));
	}

	return newBoard;
}
