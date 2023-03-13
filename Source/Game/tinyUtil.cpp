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

int Ship::getHealth() const { return int_health_; }

void Ship::damaged() {
    // this->SetFrameIndexOfBitmap(this->GetFrameIndexOfBitmap() + 2);
}

void Ship::sink() {
    // this->SetFrameIndexOfBitmap(this->GetFrameIndexOfBitmap() + 4);
}

void Ship::beingHit() { if (--int_health_ == 0) { this->sink(); } else { this->damaged(); } }

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
    const string baseAddress = "Resources/ships/";
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

int gameBoard::getCurrSel() const { return currently_sel_ship_; }

vector<Ship*> gameBoard::getShip() { return ships_; }

void gameBoard::pickUpShip(const int& sel) {
    currently_sel_ship_ = sel;
    const int x = (ships_.at(sel)->GetLeft() - base_x_) / 60;
    const int y = (ships_.at(sel)->GetTop() - base_x_) / 60;
    const int direction = ships_.at(sel)->GetFrameIndexOfBitmap();
    for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
        if (x < 10 && y < 10) {
            auto grid = new EmptyGrid;
            // ReSharper disable CppStringLiteralToCharPointerConversion
            grid->LoadBitmapA(R"(Resources/emptyGrid.bmp)");
            // ReSharper restore CppStringLiteralToCharPointerConversion
            switch (direction) {
                case 0:
                    grids_.at(x + i).at(y)->shipPickingUpHere();
                    break;
                case 1:
                    grids_.at(x).at(y + i)->shipPickingUpHere();
                    break;
                default:
                    break;
            }
        }
    }
}

void gameBoard::dropShip(const CPoint& pt) {
    const int x = (pt.x - base_x_) / 60;
    const int y = (pt.y - base_y_) / 60;
    if (x > 9 || y > 9) return;
    int direction = ships_.at(currently_sel_ship_)->GetFrameIndexOfBitmap();
    bool flag = grids_.at(x).at(y)->ifPlaceable();
    for (int i = 1; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
        switch (direction) {
            case 0:
                if (x + i >= 10 || !grids_.at(x + i).at(y)->ifPlaceable()) flag = false;
                break;
            case 1:
                if (y + i >= 10 || !grids_.at(x).at(y + i)->ifPlaceable()) { flag = false; }
                break;
            default: ;
        }
    }
    if (flag) {
        ships_.at(currently_sel_ship_)->SetTopLeft(grids_.at(x).at(y)->GetLeft(), grids_.at(x).at(y)->GetTop());
        for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
            auto grid = new BaseGrid;
            switch (direction) {
                case 0:
                    *grid = *grids_.at(x + i).at(y);
                    delete grids_.at(x + i).at(y);
                    grids_.at(x + i).at(y) = grid;
                    grids_.at(x + i).at(y)->shipDroppingHere(currently_sel_ship_);

                    break;
                case 1:
                    *grid = *grids_.at(x).at(y + i);
                    delete grids_.at(x).at(y + i);
                    grids_.at(x).at(y + i) = grid;
                    grids_.at(x).at(y + i)->shipDroppingHere(currently_sel_ship_);
                    break;
                default:
                    break;
            }
        }
        currently_sel_ship_ = -1;
    }// else: may have to do something    
}

bool gameBoard::ifAllShipPlaced() const {
    if (currently_sel_ship_ != -1) return false;
    for (auto& ship: ships_) { if (ship->GetLeft() >= base_x_ + 10 * 60) return false; }
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
    // i'm too lazy to write a copy constructor, so player1's board is stolen by player2
    // TODO write a copy constructor
    this->base_x_ = SIZE_X - 150 - 60 * 10;
    this->base_y_ = 150;
    this->currently_sel_ship_ = -1;
    this->grids_ = copied.grids_;
    this->ships_ = copied.ships_;
    for (auto& grid: grids_) { for (auto& g: grid) { g->SetTopLeft(g->GetLeft() + 1020, g->GetTop()); } }
    for (auto& ship: ships_) { ship->SetTopLeft(ship->GetLeft() + 1020, ship->GetTop()); }

}

bool gameBoard::beingHit(const int& x, const int& y) {
    if (grids_.at(x).at(y)->GetFrameIndexOfBitmap() == grids_.at(x).at(y)->GetFrameSizeOfBitmap() - 1) return false;
    grids_.at(x).at(y)->SetFrameIndexOfBitmap(grids_.at(x).at(y)->GetFrameSizeOfBitmap() - 1);
    if (grids_.at(x).at(y)->getShipID() != -1) { ships_.at(grids_.at(x).at(y)->getShipID())->beingHit(); }
    return true;
}

bool gameBoard::ifAllShipSunk() const {
    for (auto& i: ships_) { if (i->getHealth() != 0) return false; }
    return true;
}

void gameBoard::init() {
    currently_sel_ship_ = -1;
    base_y_ = base_x_ = 150;
    vector<string> fileName = {"Resources/emptyGrid.bmp", "Resources/gridHit.bmp"};
    for (int i = 0; i < 10; ++i) {
        vector<BaseGrid*> curr(10);
        for (int j = 0; j < 10; ++j) {
            curr.at(j) = new EmptyGrid;
            curr.at(j)->LoadBitmapByString(fileName);
            curr.at(j)->SetTopLeft(base_x_, j * 60 + base_y_);
        }
        grids_.push_back(curr);
        base_x_ += 60;
    }
    for (int i = 2; i < 6; ++i) {
        ships_.emplace_back(MakeAShip(i));
        ships_.back()->SetTopLeft(base_x_, (i - 2) * 60 + base_y_);
    }
    ships_.emplace_back(MakeAShip((9)));
    ships_.back()->SetTopLeft(base_x_, base_y_ + 240);
    base_y_ = base_x_ = 150;

}

void gameBoard::show() {
    if (currently_sel_ship_ == -1) for (auto& i: ships_) { i->ShowBitmap(); }
    for (auto& i: grids_) { for (auto& j: i) { if (j->ifDisplay()) j->ShowBitmap(); } }
    if (currently_sel_ship_ != -1) for (auto& i: ships_) { i->ShowBitmap(); }
}

void gameBoard::rotateShip() {
    if (currently_sel_ship_ == -1) return;
    ships_.at(currently_sel_ship_)->rotate();
}
