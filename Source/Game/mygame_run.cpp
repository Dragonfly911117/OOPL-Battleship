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

void CGameStateRun::OnMove() // 移動遊戲元素
{

}

void CGameStateRun::OnInit() {
    menu_bkg_.LoadBitmapA("Resources/menuBg.bmp");
    menu_bkg_.SetTopLeft(0, 0);
    for (int i = 0; i < 4; ++i) {
        menu_btns[i].LoadBitmapA("Resources/MenuBtn.bmp");
        menu_btns[i].SetTopLeft((int)(SIZE_X * 0.4),(int) (SIZE_Y * 0.2 * (i)+200));
    }
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) // 處理滑鼠的動作
{
}

void CGameStateRun::OnShow() {
    switch (int_phase_) {
    case menu:
        menu_bkg_.ShowBitmap();
        for (auto i : menu_btns)
            i.ShowBitmap();
        
        break;
        
    default:
        break;
    }

}

