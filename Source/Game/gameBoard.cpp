#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

void gameBoard::pickUpShip(const int& sel) {
    currently_sel_ship_ = sel;
    const int x = (ships_.at(sel)->GetLeft() - base_x_) / 60;
    const int y = (ships_.at(sel)->GetTop() - base_x_) / 60;
    const int direction = ships_.at(sel)->GetFrameIndexOfBitmap();
    for (int i = 0; i < ships_.at(currently_sel_ship_)->getSize(); ++i) {
        if (x < 10 && y < 10) {
            const auto grid = new EmptyGrid;
            grid->LoadBitmapA(R"(Resources/emptyGrid.bmp)");
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
    if (x < 0 || x > 9 || y < 0 || y > 9) return;
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
    if (grids_.at(x).at(y)->getShipID() != -1) {
        ships_.at(grids_.at(x).at(y)->getShipID())->beingHit();
        auto temp = new BaseGrid;
        temp->LoadBitmapA("Resources/shipHit.bmp");
        temp->SetTopLeft(grids_.at(x).at(y)->GetLeft(), grids_.at(x).at(y)->GetTop());
        ship_hit_.push_back(temp);
    }
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
    if (! is_enemy_) for (auto& i: ships_) { i->ShowBitmap(); }
    for (auto& i: grids_) { for (auto& j: i) { if (j->ifDisplay()) j->ShowBitmap(); } }
    if (currently_sel_ship_ != -1 && !is_enemy_) ships_.at(currently_sel_ship_)->ShowBitmap();
    for (const auto& i: ship_hit_) i->ShowBitmap();
}

void gameBoard::rotateShip() {
    if (currently_sel_ship_ == -1) return;
    ships_.at(currently_sel_ship_)->rotate();
}
