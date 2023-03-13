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

myBtn::myBtn() { this->text = ""; }

void myBtn::setText(const string& str) { this->text = str; }

void myBtn::pressed() { this->SetFrameIndexOfBitmap(1); }

void myBtn::released() { this->SetFrameIndexOfBitmap(0); }

void myBtn::showBtn() {
    this->ShowBitmap();
    CDC* pDC = CDDraw::GetBackCDC();

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));

    /* 變更字體 */
    if (this->GetFrameIndexOfBitmap() == 0) CTextDraw::ChangeFontLog(pDC, 40, "ink free", RGB(255, 255, 255), 40);
    else CTextDraw::ChangeFontLog(pDC, 40, "ink free", RGB(188, 188, 188), 40);
    CTextDraw::Print(pDC, this->GetLeft() + 10, this->GetTop() + GetHeight() / 2, this->text);
    CDDraw::ReleaseBackCDC();

}

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

int Ship::getSize() {
    // {2, 3, 4, 5, 9} ->{ 2, 3, 3, 4, 5}
    switch (int_type_) {
        case 2:
            return 2;
        // [[fallthrough]];
        case 3:
        case 4:
            return 3;
        case 5:
            return 4;
        case 9:
            return 5;
        default:
            exit(-2);
    }

}

void Ship::rotate() { this->SetFrameIndexOfBitmap((this->GetFrameIndexOfBitmap() + 1) % 2); }

Ship* MakeAShip(const int& tp) {
    auto ship = new Ship;
    ship->int_type_ = tp;
    ship->int_health_ = ship->getSize();
    ship->placeable = false;
    string baseAddress = "Resources/ships/";
    vector<string> fileNames;
    fileNames.emplace_back("headAtLeft.bmp");
    fileNames.emplace_back("headAtTop.bmp");
    switch (tp) {
        case 2:
            for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "A/" + fileNames.at(i); }
            break;
        case 3:
            for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "B/" + fileNames.at(i); }
            break;
        case 4:
            for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "C/" + fileNames.at(i); }
            break;
        case 5:
            for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "D/" + fileNames.at(i); }
            break;
        case 9:
            for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "E/" + fileNames.at(i); }
            break;
        default:
            exit(-1);
    }
    fileNames.push_back("Resources/gridHit.bmp");
    ship->LoadBitmapByString(fileNames, RGB(255, 255, 255));
    // ship->SetFrameIndexOfBitmap(1);
    return ship;
}

Ship* stealAShip(Ship* ship) {
    Ship* newShip = MakeAShip(ship->int_type_);
    newShip->shipID = ship->shipID;
    newShip->SetFrameIndexOfBitmap(ship->GetFrameIndexOfBitmap());
    newShip->SetTopLeft(ship->GetLeft() + 1020, ship->GetTop());
    return newShip;
}

int gameBoard::getCurrSel() { return currentlySelShip; }

vector<Ship*> gameBoard::getShip() { return ships; }

void gameBoard::pickUpShip(const int& sel) {
    currentlySelShip = sel;
    const int x = (ships.at(sel)->GetLeft() - baseX) / 60;
    const int y = (ships.at(sel)->GetTop() - baseX) / 60;
    const int direction = ships.at(sel)->GetFrameIndexOfBitmap();
    for (int i = 0; i < ships.at(currentlySelShip)->getSize(); ++i) {
        if (x < 10 && y < 10) {
            auto grid = new EmptyGrid;
            grid->LoadBitmapA("Resources/emptyGrid.bmp");
            switch (direction) {
                case 0:
                    grids.at(x + i).at(y)->shipPickingUpHere();
                    break;
                case 1:
                    grids.at(x).at(y + i)->shipPickingUpHere();
                    break;
                default:
                    break;
            }
        }
    }
}

void gameBoard::dropShip(const CPoint& pt) {
    const int x = (pt.x - baseX) / 60;
    const int y = (pt.y - baseY) / 60;
    if (x > 9 || y > 9) return;
    int direction = ships.at(currentlySelShip)->GetFrameIndexOfBitmap();
    bool flag = grids.at(x).at(y)->ifPlaceable();
    for (int i = 1; i < ships.at(currentlySelShip)->getSize(); ++i) {
        switch (direction) {
            case 0:
                if (x + i >= 10 || !grids.at(x + i).at(y)->ifPlaceable()) flag = false;
                break;
            case 1:
                if (y + i >= 10 || !grids.at(x).at(y + i)->ifPlaceable()) { flag = false; }
                break;
        }
    }
    if (flag) {
        ships.at(currentlySelShip)->SetTopLeft(grids.at(x).at(y)->GetLeft(), grids.at(x).at(y)->GetTop());
        for (int i = 0; i < ships.at(currentlySelShip)->getSize(); ++i) {
            auto grid = new BaseGrid;
            switch (direction) {
                case 0:
                    *grid = *grids.at(x + i).at(y);
                    delete grids.at(x + i).at(y);
                    grids.at(x + i).at(y) = grid;
                    grids.at(x + i).at(y)->shipDroppingHere(currentlySelShip);

                    break;
                case 1:
                    *grid = *grids.at(x).at(y + i);
                    delete grids.at(x).at(y + i);
                    grids.at(x).at(y + i) = grid;
                    grids.at(x).at(y + i)->shipDroppingHere(currentlySelShip);
                    break;
            }
        }
        currentlySelShip = -1;
    }// else: may have to do something    
}

bool gameBoard::ifAllShipPlaced() {
    if (currentlySelShip != -1) return false;
    for (auto& ship: ships) { if (ship->GetLeft() >= baseX + 10 * 60) return false; }
    return true;
}

void gameBoard::gettingStart() {

    // wah
    // for (int i = 0; i < ships.size(); ++i) {
    //     const int direction = ships.at(i)->GetFrameIndexOfBitmap();
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

void gameBoard::whatEasesMyPainCannotBeCalledSteal(const gameBoard& copied) {
    this->baseX = SIZE_X - 150 - 60 * 10;
    this->baseY = 150;
    this->currentlySelShip = -1;
    vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};

    for (int i = 0; i < 10; ++i) {
        vector<BaseGrid*> curr(10);
        for (int j = 0; j < 10; ++j) {
            curr.at(j) = new EmptyGrid;
            curr.at(j)->LoadBitmapByString(fileName);
            curr.at(j)->SetTopLeft(baseX, j * 60 + baseY);
            curr.at(j)->setShipID(copied.grids.at(i).at(j)->getShipID());
        }
        grids.push_back(curr);
        baseX += 60;
    }
    for (auto ship: copied.ships) {
        ships.emplace_back(stealAShip(ship));
        for (int i = 0; i < ships.back()->getSize(); ++i) { }
    }
}

bool gameBoard::beingHit(const int& x, const int& y) {
    if (grids.at(x).at(y)->GetFrameIndexOfBitmap() == grids.at(x).at(y)->GetFrameSizeOfBitmap() - 1) return false;
    grids.at(x).at(y)->SetFrameIndexOfBitmap(grids.at(x).at(y)->GetFrameSizeOfBitmap() - 1);
    return true;
}

void gameBoard::init() {
    currentlySelShip = -1;
    baseY = baseX = 150;
    vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};
    for (int i = 0; i < 10; ++i) {
        vector<BaseGrid*> curr(10);
        for (int j = 0; j < 10; ++j) {
            curr.at(j) = new EmptyGrid;
            curr.at(j)->LoadBitmapByString(fileName);
            curr.at(j)->SetTopLeft(baseX, j * 60 + baseY);
        }
        grids.push_back(curr);
        baseX += 60;
    }
    for (int i = 2; i < 6; ++i) {
        ships.emplace_back(MakeAShip(i));
        ships.back()->SetTopLeft(baseX, (i - 2) * 60 + baseY);
    }
    ships.emplace_back(MakeAShip((9)));
    ships.back()->SetTopLeft(baseX, baseY + 240);
    baseY = baseX = 150;

}

void gameBoard::show() {
    if (currentlySelShip == -1) for (auto& i: ships) { i->ShowBitmap(); }
    for (auto& i: grids) { for (auto& j: i) { if (j->ifDisplay()) j->ShowBitmap(); } }
    if (currentlySelShip != -1) for (auto& i: ships) { i->ShowBitmap(); }
}

void gameBoard::rotateShip() {
    if (currentlySelShip == -1) return;
    ships.at(currentlySelShip)->rotate();
}
