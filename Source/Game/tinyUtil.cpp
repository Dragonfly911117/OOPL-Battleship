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

myBtn::myBtn() {
    this->text = "";
}

void myBtn::setText(const string& str) {
    this->text = str;
}

void myBtn::pressed() {
    this->SetFrameIndexOfBitmap(1);
}

void myBtn::released() {
    this->SetFrameIndexOfBitmap(0);
}

void myBtn::showBtn() {
    this->ShowBitmap();
    CDC* pDC = CDDraw::GetBackCDC();

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));

    /* 變更字體 */
    CTextDraw::ChangeFontLog(pDC, 40, "ink free", RGB(255, 255, 255), 40);
    CTextDraw::Print(pDC, this->GetLeft() + 10, this->GetTop() + GetHeight() / 2, this->text);
    CDDraw::ReleaseBackCDC();

}

bool BaseGrid::ifPlaceable() {
    return placeable;
}

Ship* makeAShip(const int& tp) {
    Ship* ship = new Ship;
    ship->type_ = tp;
    ship->health_ = tp % 6;
    ship->picked_ = false;
    ship->placeable = false;
    ship->LoadBitmapByString({"Resources/shipFullHealth.bmp", "Resources/shipDamaged.bmp", "Resources/shipSink.bmp"});
    return ship;
}

int gameBoard::getCurrSel() {
    return currentlySelShip;
}

vector<Ship*> gameBoard::getShip() {
    return ships;
}

void gameBoard::pickUpShip(const int& i) {
    currentlySelShip = i;
    int x, y;
    x = (ships.at(i)->GetLeft() - baseX) / 60;
    y = (ships.at(i)->GetTop() - baseX) / 60;
    if (x < 10 && y < 10) {
        grids.at(x).at(y) = new EmptyGrid;
        grids.at(x).at(y)->LoadBitmapA("Resources/emptyGrid.bmp");
        grids.at(x).at(y)->SetTopLeft(ships.at(i)->GetLeft(), ships.at(i)->GetTop());
    }
}

void gameBoard::dropShip(const CPoint& pt) {
    int x, y;
    x = (pt.x - baseX) / 60;
    y = (pt.y - baseY) / 60;
    if (grids.at(x).at(y)->ifPlaceable()) {
        ships.at(currentlySelShip)->SetTopLeft(grids.at(x).at(y)->GetLeft(), grids.at(x).at(y)->GetTop());
        delete grids.at(x).at(y);
        grids.at(x).at(y) = ships.at(currentlySelShip);
        currentlySelShip = -1;
    } // else: may have to do something    
}

void gameBoard::init() {
    currentlySelShip = -1;
    baseY = baseX = 0;
    for (int i = 0; i < 10; ++i) {
        vector<BaseGrid*> curr(10);
        for (int j = 0; j < 10; ++j) {
            curr.at(j) = new EmptyGrid;
            curr.at(j)->LoadBitmapA("Resources/emptyGrid.bmp");
            curr.at(j)->SetTopLeft(baseX, j * 60);
        }
        grids.push_back(curr);
        baseX += 60;
    }
    for (int i = 2; i < 6; ++i) {
        ships.emplace_back(makeAShip(i));
        ships.back()->SetTopLeft(baseX, (i - 1) * 60);
    }
    ships.emplace_back(makeAShip((9)));
    ships.back()->SetTopLeft(baseX, baseY);
    baseX = baseY = 0;
}

void gameBoard::show() {
    for (auto& i : grids) {
        for (auto& j : i) {
            j->ShowBitmap();
        }
    }
    for (auto& i : ships) {
        i->ShowBitmap();
    }
}
