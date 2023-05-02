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
	startBg.LoadBitmapA("Resources/startBG.bmp");
	startBg.SetTopLeft(0, 0);
	CAudio* audio = CAudio::Instance();
	audio->Load(AudioID::theme, "Resources/audio/Theme.wav");
	audio->Load(AudioID::sad_violin, "Resources/audio/Sad_violin.wav");
	for (int i = 0; i < AudioID::hit_buffer; i++) {
		audio->Load(AudioID::player_hit + i, "Resources/audio/Hit_Marker.wav");
		audio->Load(AudioID::bot_hit + i, "Resources/audio/Error.wav");
	}
	audio->Load(AudioID::click, "Resources/audio/Click.wav");
	audio->Load(AudioID::defeated, "Resources/audio/Defeated.wav");
	// audio->Play(AudioID::defeated);
	// audio->Play(AudioID::theme);
}

void CGameStateInit::OnBeginState() {
	
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
	CAudio* audio = CAudio::Instance();
	audio->Stop(AudioID::theme);
	GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow() {
	startBg.ShowBitmap();
	
	
}
