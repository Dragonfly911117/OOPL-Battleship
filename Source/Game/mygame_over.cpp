#include "audioIDEnum.h"
#include "stdafx.h"
#include "config.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
	: CGameState(g) {
}

void CGameStateOver::OnMove() {
	exit(0);
}

void CGameStateOver::OnBeginState() {
}

void CGameStateOver::OnInit() {
	
	 
}

void CGameStateOver::OnShow() {
}
