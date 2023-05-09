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
}
