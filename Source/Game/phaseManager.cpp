#include "stdafx.h"
#include "phaseManager.h"
PhaseManager_global::PhaseManager_global(const initializer_list<CMovingBitmap*>& objs) {
    _background = *objs.begin();
    _cursor = *(objs.begin() + 1);
}

void PhaseManager_global::init() {
    _background->LoadBitmapA("Resources/menuBg.bmp");
    _background->SetTopLeft(0, 0);

    _cursor->LoadBitmapA("Resources/cursor.bmp");
    
}

PhaseManager_menu::PhaseManager_menu(const initializer_list<myBtn*>& buttons)
    : _buttons(buttons) {}

void PhaseManager_menu::init() {}

PhaseManager_placement::PhaseManager_placement(GameBoard* const board1, myBtn* const gameStartButton)
    : _board1(board1),
      _gameStartButton(gameStartButton) {}

void PhaseManager_placement::init() {}
