#include "stdafx.h"
#include "phaseManager.h"
#include "buttons.h"
PhaseManager_global::PhaseManager_global(const vector<CMovingBitmap*>& objs) {
    this->_bg = *objs.begin();
    this->_cursor = *(objs.begin() + 1);
}

void PhaseManager_global::init() {
    this->_bg->LoadBitmapA("Resources/menuBg.bmp");
    this->_bg->SetTopLeft(0, 0);
    
    this->_cursor->LoadBitmapA("Resources/cursor.bmp");
}

PhaseManager_menu::PhaseManager_menu(const initializer_list<CMovingBitmap*>& objs) {
    for (int i = 0; i < objs.size(); ++i) { // size = 4
        this->_buttons.emplace_back(*(objs.begin() + i));
        
    }
}

void PhaseManager_menu::init() {
    for (auto& i : _buttons) {
        i->LoadBitmapByString(this->_buttonPath);
    }
        
}
