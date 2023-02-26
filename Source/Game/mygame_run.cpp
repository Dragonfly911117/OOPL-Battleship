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
    // TODO : enter corresponding phases
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

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (nChar == 27) exit(1);

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

    default:
        break;
    }
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
    cursor.SetTopLeft(point.x - 5, point.y - 5);
    switch (int_phase_) {
    case menu:
        for (auto& i : menu_btns) {
            if (CMovingBitmap::IsOverlap(cursor, i)) {
                i.released();
            }
            else if (i.GetFrameIndexOfBitmap() == 1)
                i.SetFrameIndexOfBitmap(0);
        }

    default:
        break;
    }
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
    cursor.SetTopLeft(point.x - 5, point.y - 5);
    // cursor.ShowBitmap();
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

    default:
        break;
    }

    // cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}
