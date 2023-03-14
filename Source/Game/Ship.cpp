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

int gameBoard::getCurrSel() const { return currently_sel_ship_; }

vector<Ship*> gameBoard::getShip() { return ships_; }
