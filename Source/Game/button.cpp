#include "stdafx.h"
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "buttons.h"

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

void buttonsInit(myBtn menuBtn[4], myBtn& startButton) {

    for (int i = 0; i < 4; ++i) {
        menuBtn[i].LoadBitmapByString({"Resources/Btn.bmp", "Resources/BtnBeingPressed.bmp"});
        menuBtn[i].SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * (i) + 200)));
    }
    menuBtn[0].setText("Single Player");
    menuBtn[1].setText("Multiple Players");
    menuBtn[2].setText("Options");
    menuBtn[3].setText("Exit");
}
