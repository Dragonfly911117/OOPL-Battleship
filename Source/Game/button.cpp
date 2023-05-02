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
#include "audioIDEnum.h"
#include "buttons.h"

myBtn::myBtn() {
	this->_text = "";
}

void myBtn::setText(const string& str) {
	this->_text = str;
}

void myBtn::pressed() {
	this->SetFrameIndexOfBitmap(1);
}

void myBtn::released() {
	CAudio* audio = CAudio::Instance();
	audio->Play(AudioID::click);
	this->SetFrameIndexOfBitmap(0);
}

void myBtn::showBtn() {
	this->ShowBitmap();
	CDC* pDC = CDDraw::GetBackCDC();

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));

	/* 變更字體 */
	if (this->GetFrameIndexOfBitmap() == 0) {
		CTextDraw::ChangeFontLog(pDC, 40, "ink free", RGB(255, 255, 255), 40);
	} else {
		CTextDraw::ChangeFontLog(pDC, 40, "ink free", RGB(188, 188, 188), 40);
	}
	// make the text is in the middle of the button's height
	CTextDraw::Print(pDC, this->GetLeft() + 10, this->GetTop() + GetHeight() / 2, this->_text);

	CDDraw::ReleaseBackCDC();
}
