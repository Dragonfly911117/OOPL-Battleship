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
    if (int_type_ < 4) return int_type_;
    if (int_type_ < 6) return int_type_ - 1;
    if (int_type_ == 9) return 5;
    exit(-1);
}

void Ship::rotate() { this->SetFrameIndexOfBitmap((this->GetFrameIndexOfBitmap() + 1) % 2); }

int gameBoard::getCurrSel() const { return currently_sel_ship_; }

vector<Ship*> gameBoard::getShip() { return ships_; }

Ship* MakeAShip(const int& tp) {
    const auto ship = new Ship;
    ship->int_type_ = tp;
    ship->int_health_ = ship->getSize();
    ship->placeable = false;
    const string baseAddress = "Resources/ships/";
    vector<string> fileNames = {"headAtLeft.bmp", "headAtTop.bmp", "Resources/gridHit.bmp"};
    string folderName("A/");
    folderName[0] += tp - 2;// {2, 3, 4, 5, 9} ->{ 'A', 'B', 'C', 'D', 'H'}
    for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + folderName + fileNames.at(i); }
    // switch (tp) {
    //     case 2:
    //
    //         break;
    //     case 3:
    //         for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "B/" + fileNames.at(i); }
    //         break;
    //     case 4:
    //         for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "C/" + fileNames.at(i); }
    //         break;
    //     case 5:
    //         for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "D/" + fileNames.at(i); }
    //         break;
    //     case 9:
    //         for (int i = 0; i < 2; ++i) { fileNames.at(i) = baseAddress + "E/" + fileNames.at(i); }
    //         break;
    //     default:
    //         exit(-1);
    // }
    ship->LoadBitmapByString(fileNames);
    // ship->SetFrameIndexOfBitmap(1);
    return ship;
}

Ship* copyAShip(Ship* ship) {
    Ship* newShip = MakeAShip(ship->int_type_);
    newShip->shipID = ship->shipID;
    newShip->SetFrameIndexOfBitmap(ship->GetFrameIndexOfBitmap());
    newShip->SetTopLeft(ship->GetLeft() + 1020, ship->GetTop());
    return newShip;
}
