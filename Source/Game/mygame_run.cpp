#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////


CGameStateRun::CGameStateRun(CGame* g) : CGameState(g) {

}

CGameStateRun::~CGameStateRun() {

}

void CGameStateRun::OnBeginState() {

}

void CGameStateRun::OnMove() {

}

void CGameStateRun::OnInit() {
    cursor.LoadBitmapA("Resources/cursor.bmp");
    menu_bkg_.LoadBitmapA("Resources/menuBg.bmp");
    menu_bkg_.SetTopLeft(0, 0);
    for (int i = 0; i < 4; ++i) {
        menu_btns[i].LoadBitmapByString({"Resources/menuBtn.bmp", "Resources/menuBtnBeingPressed.bmp"});
        menu_btns[i].SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * (i) + 200)));
    }
    menu_btns[0].setText("Single Player");
    menu_btns[1].setText("Multiple Players");
    menu_btns[2].setText("Options");
    menu_btns[3].setText("Exit");

    board.init();

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (int_phase_ == single_game) {
        if (board.getCurrSel() != -1) {
            if (nChar == 52 || nChar == 82) {
                 board.rotateShip();
            }
        }
    }
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (nChar == 27) exit(27);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {
    cursor.SetTopLeft(point.x - 5, point.y - 5);
    switch (int_phase_) {
    case menu:
        for (auto& i : menu_btns) {
            if (CMovingBitmap::IsOverlap(cursor, i)) {
                i.pressed();
                break;
            }
        }
    case single_game:
        if (board.getCurrSel() == -1) {
            const auto ships = board.getShip();
            for (int i = 0; i < 5; ++i) {
                if (CMovingBitmap::IsOverlap(cursor, *ships.at(i))) {
                    board.pickUpShip(i);
                    break;
                }
            }
        }
        else {
            const auto ships = board.getShip();
            board.dropShip(point);
        }
        break;
    default:
        break;
    }
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
    cursor.SetTopLeft(point.x - 5, point.y - 5);
    switch (int_phase_) {
    case menu:
        for (int i = 0; i < 4; ++i){
            if (CMovingBitmap::IsOverlap(cursor, menu_btns[i])) {
                menu_btns[i].released();
                
                // Remember to remove these two lines
                startSingleGame();
                return;
                
                switch (i) {
                case  0 :
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
            else if (menu_btns[i].GetFrameIndexOfBitmap() == 1)
                menu_btns[i].released();
        }

    default:
        break;
    }
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
    cursor.SetTopLeft(point.x - 5, point.y - 5);
switch (int_phase_) {
    case menu: // copilot suggested code, add dis function in the future
        // for (auto& i : menu_btns) {
        //     if (CMovingBitmap::IsOverlap(cursor, i)) {
        //         i.hover();
        //         break;
        //     }
        //     else if (i.GetFrameIndexOfBitmap() == 1)
        //         i.released();
        // }
        break;
    case single_game:
        if (board.getCurrSel() != -1) {
            board.getShip().at(board.getCurrSel())->SetTopLeft(point.x - 25, point.y - 25);
        }
        break;
    default:
        break;
    }
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {
}

void CGameStateRun::OnShow() {

    switch (int_phase_) {
    case menu:
        menu_bkg_.ShowBitmap();
        for (auto& i : menu_btns)
            i.showBtn();
        break;
    case single_game:
        board.show();
    default:
        break;
    }

    // cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}

void CGameStateRun::startSingleGame() {
    int_phase_ = single_game;
}

void CGameStateRun::start_mutiple_game() {
    int_phase_ = match_making;
}

void CGameStateRun::gotoSettings() {
    int_phase_ = settings;
}

void CGameStateRun::gotoExit() {
    GotoGameState(GAME_STATE_OVER);
}
