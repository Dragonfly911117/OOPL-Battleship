#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
// #include "tinyUtil.h"
#include "phaseManager.h"
using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g) {}

CGameStateRun::~CGameStateRun() {}

void CGameStateRun::OnBeginState() {}

void CGameStateRun::OnMove() {}

void CGameStateRun::OnInit() {
    vector<CMovingBitmap*> temp = {&this->_backgrounds, &this->_cursor};
    this->_phaseManagers.emplace_back(new PhaseManager_global({&this->_backgrounds, &this->_cursor}));
    this->_phaseManagers.emplace_back(new PhaseManager_menu({this->_menuButton, this->_menuButton + 1, this->_menuButton + 2, this->_menuButton + 3}));
    this->_phaseManagers.push_back(new PhaseManager_placement(&this->_player1Board, {&this->_gameStartButton}));
    for (const auto& i: this->_phaseManagers) { i->init(); }

    // _cursor.LoadBitmapA("Resources/cursor.bmp");
    // _backgrounds.LoadBitmapA("Resources/menuBg.bmp");
    // _backgrounds.SetTopLeft(0, 0);

    // buttonsInit(_menuButton, _gameStartButton);
    // _gameStartButton.LoadBitmapByString({"Resources/button.bmp", "Resources/buttonPressed.bmp"});
    // _gameStartButton.SetTopLeft(SIZE_X - 150 - _gameStartButton.GetWidth(), SIZE_Y - 150 - _gameStartButton.GetHeight());
    // _gameStartButton.setText("Game Start!");
    _player1Board.init();

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) { if (_phase == placement_phase) { if (_player1Board.getCurrSel() != -1) { if (nChar == 52 || nChar == 82) { _player1Board.rotateShip(); } } } }

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { if (nChar == 27) exit(27); }

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {
    _cursor.SetTopLeft(point.x - 5, point.y - 5);
    int x1 = (point.x - 150) / 60;
    int x2 = (point.x - (SIZE_X - 150 - 60 * 10)) / 60;
    int y = (point.y - 150) / 60;
    switch (_phase) {
        case menu:
            for (auto& i: _menuButton) {
                if (CMovingBitmap::IsOverlap(_cursor, i)) {
                    i.pressed();
                    break;
                }
            }
        case placement_phase:
            if (_player1Board.getCurrSel() == -1) {
                const auto ships = _player1Board.getShip();
                for (int i = 0; i < 5; ++i) {
                    // if (myIsOverlap(point, ships.at(i)) != 0) {
                    if (CMovingBitmap::IsOverlap(_cursor, *ships.at(i))) {
                        _player1Board.pickUpShip(i);
                        break;
                    }
                }
            } else { _player1Board.dropShip(point); }
            if (_player1Board.ifAllShipPlaced()) { if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton)) { _gameStartButton.pressed(); } }
            break;
        case in_game:
            if (y < 0 || y > 9) break;
            if ((x2 < 0 || x2 > 9 && _turnFlag) && (x1 < 0 || x1 > 9 && !_turnFlag)) break;

            _turnFlag ? player1Turn(x2, y) : player2Turn(x1, y);
            break;
        default:
            break;
    }
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
    _cursor.SetTopLeft(point.x - 5, point.y - 5);
    switch (_phase) {
        case menu:
            
            for (int i = 0; i < 4; ++i) {
                if (CMovingBitmap::IsOverlap(_cursor, _menuButton[i]) && _menuButton[i].GetFrameIndexOfBitmap() == 1) {

                    // Remember to remove these two lines
                    startSingleGame();
                    return;

                    switch (i) {
                        case 0:
                            startSingleGame();
                            break;
                        case 1:
                            start_mutiple_game();
                            break;
                        case 2:
                            gotoSettings();
                            break;
                        case 3:
                            gotoExit();
                            break;
                        default:
                            break;
                    }
                }
            }
        case placement_phase:
            if (_player1Board.ifAllShipPlaced()) {
                if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton)) {
                    _gameStartButton.released();
                    gameStart();
                    // start game
                }
            }
        default:
            break;
    }
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
    _cursor.SetTopLeft(point.x - 5, point.y - 5);
    switch (_phase) {
        case menu: // copilot suggested code, add this function in the future
            // for (auto& i : menu_btns) {
            //     if (CMovingBitmap::IsOverlap(cursor, i)) {
            //         i.hover();
            //         break;
            //     }
            //     else if (i.GetFrameIndexOfBitmap() == 1)
            //         i.released();
            // }
            // for (auto& i: _menuButton) {
            //     if (CMovingBitmap::IsOverlap(_cursor, i)) {
            //         break;
            //     }
            //     i.released();
            // }
            break;
        case placement_phase:
            if (_player1Board.getCurrSel() != -1) { _player1Board.getShip().at(_player1Board.getCurrSel())->SetTopLeft(point.x - 25, point.y - 25); }
            break;

        default:
            break;
    }
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {}

void CGameStateRun::OnShow() {
    switch (_phase) {
        case menu:
            _backgrounds.ShowBitmap();
            for (auto& i: _menuButton) i.showBtn();
            break;
        case placement_phase:
            _player1Board.show();
            if (_player1Board.ifAllShipPlaced()) _gameStartButton.showBtn();
        case in_game:
            _player1Board.show();
            _player2Board.show();
        default:
            break;
    }

    // cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}

void CGameStateRun::startSingleGame() { _phase = placement_phase; }

void CGameStateRun::start_mutiple_game() { _phase = multiply_players; }

void CGameStateRun::gameStart() {
    // copy _player1Board for bot's usage, and start the game Done!
    // make sure the bot's _player1Board is shown but not its ships  Done!
    // _player2Board.whatEasesMyPainCannotBeCalledSteal(_player1Board); 
    _player2Board = copyABoard(_player1Board);
    _phase = in_game;
}

void CGameStateRun::player1Turn(const int& x, const int& y) {
    if (x < 0 || x > 9 || y < 0 || y > 9) return;
    if (_player2Board.beingHit(x, y)) {
        if (_player2Board.ifAllShipSunk()) _phase = we_have_a_winner;
        else _turnFlag = false;
    }
}

void CGameStateRun::player2Turn(const int& x, const int& y) {
    if (x < 0 || x > 9 || y < 0 || y > 9) return;
    if (_player1Board.beingHit(x, y)) {
        if (_player1Board.ifAllShipSunk()) _phase = we_have_a_winner;
        else _turnFlag = true;
    }
}

void CGameStateRun::gotoSettings() { _phase = settings; }

void CGameStateRun::gotoExit() { GotoGameState(GAME_STATE_OVER); }
