#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "audioIDEnum.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
	: CGameState(g) {
}

void CGameStateInit::OnInit() {
	ShowInitProgress(0, "Start Initializing...");// 一開始的loading進度為0%
	startBg.LoadBitmapA(R"(Resources/Images/Backgrounds/gameInit.bmp)");
	startBg.SetTopLeft(0, 0);
	CAudio* audio = CAudio::Instance();
	audio->Load(AudioID::theme, R"(Resources/audio/Theme.wav)");

	// choose one to use
	audio->Load(AudioID::sad_violin, R"(Resources/audio/Sad_violin.wav)");
	audio->Load(AudioID::defeated, R"(Resources/audio/Defeated.wav)");

	for (int i = 0; i < AudioID::hit_buffer; i++) {
		audio->Load(AudioID::player_hit + i, "Resources/audio/Hit_Marker.wav");
		audio->Load(AudioID::bot_hit + i, "Resources/audio/Error.wav");
	}
	audio->Load(AudioID::click, "Resources/audio/Click.wav");
	audio->Load(AudioID::Missed1,  "Resources/audio/Missed1.wav");
	audio->Load(AudioID::Missed2,  "Resources/audio/Missed2.wav");
	audio->Load(AudioID::Missed3,  "Resources/audio/Missed3.wav");
	audio->Load(AudioID::Missed4,  "Resources/audio/Missed4.wav");
	audio->Load(AudioID::defeat_not_DSMode_Bot, "Resources/audio/Defeat_NotDS.wav");
	audio->Load(AudioID::defeat_dark_soul, "Resources/audio/Defeat_Dark_Soul.wav");
	
	
	audio->Play(AudioID::theme, true);
}

void CGameStateInit::OnBeginState() {
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
	CAudio* audio = CAudio::Instance();
	GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow() {
	startBg.ShowBitmap();
	

}
