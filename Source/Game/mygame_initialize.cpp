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
	audio->Load(AudioID::theme, R"(Resources/Audio/Theme.wav)");

	// choose one to use
	audio->Load(AudioID::sad_violin, R"(Resources/Audio/Sad_violin.wav)");
	audio->Load(AudioID::defeated, R"(Resources/Audio/Defeated.wav)");

	for (int i = 0; i < AudioID::hit_buffer; i++) {
		audio->Load(AudioID::player_hit + i, R"(Resources/Audio/Hit_Marker.wav)");
		audio->Load(AudioID::bot_hit + i, R"(Resources/Audio/Error.wav)");
	}
	audio->Load(AudioID::click, R"(Resources/Audio/Click.wav)");
	audio->Load(AudioID::Missed1,  R"(Resources/Audio/Missed1.wav)");
	audio->Load(AudioID::Missed2,  R"(Resources/Audio/Missed2.wav)");
	audio->Load(AudioID::Missed3,  R"(Resources/Audio/Missed3.wav)");
	audio->Load(AudioID::Missed4,  R"(Resources/Audio/Missed4.wav)");
	audio->Load(AudioID::defeat_not_DSMode_Bot, R"(Resources/Audio/Defeat_NotDS.wav)");
	audio->Load(AudioID::defeat_dark_soul, R"(Resources/Audio/Defeat_Dark_Soul.wav)");
	audio->Load(AudioID::CheatModeActivated, R"(Resources/Audio/CheatModeActivated.wav)");
	
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
