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

CGameStateRun::CGameStateRun(CGame* g)
	: CGameState(g) {
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
		menu_btns_[i].LoadBitmapByString({"Resources/Btn.bmp", "Resources/BtnBeingPressed.bmp"});
		menu_btns_[i].SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * (i) + 200)));
	}
	menu_btns_[0].setText("Single Player");
	menu_btns_[1].setText("Multiple Players");
	menu_btns_[2].setText("Options");
	menu_btns_[3].setText("Exit");

	btn_start_.LoadBitmapByString({"Resources/Btn.bmp", "Resources/BtnBeingPressed.bmp"});
	btn_start_.SetTopLeft(SIZE_X - 150 - btn_start_.GetWidth(), SIZE_Y - 150 - btn_start_.GetHeight());
	btn_start_.setText("Game Start!");
	player1_board_.init();

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) { if (int_phase_ == placement_phase) { if (player1_board_.getCurrSel() != -1) { if (nChar == 52 || nChar == 82) { player1_board_.rotateShip(); } } } }

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == 27)
		exit(27);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {
	cursor.SetTopLeft(point.x - 5, point.y - 5);
	int x1 = (point.x - 150) / 60;
	int x2 = (point.x - (SIZE_X - 150 - 60 * 10)) / 60;
	int y = (point.y - 150) / 60;
	switch (int_phase_) {
		case menu:
			for (auto& i: menu_btns_) {
				if (CMovingBitmap::IsOverlap(cursor, i)) {
					i.pressed();
					break;
				}
			}
		case placement_phase:
			if (player1_board_.getCurrSel() == -1) {
				const auto ships = player1_board_.getShip();
				for (int i = 0; i < 5; ++i) {
					if (myIsOverlap(point, ships.at(i)) != 0) {
						player1_board_.pickUpShip(i);
						break;
					}
				}
			} else { player1_board_.dropShip(point); }
			if (player1_board_.ifAllShipPlaced()) { if (CMovingBitmap::IsOverlap(cursor, btn_start_)) { btn_start_.pressed(); } }
			break;
		case in_game:
			if (y < 0 || y > 9)
				break;
			if ((x2 < 0 || x2 > 9 && is_player1_turn_) && (x1 < 0 || x1 > 9 && !is_player1_turn_))
				break;

			is_player1_turn_ ? player1Turn(x2, y) : player2Turn(x1, y);
			break;
		default:
			break;
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
	cursor.SetTopLeft(point.x - 5, point.y - 5);
	switch (int_phase_) {
		case menu:
			for (int i = 0; i < 4; ++i) {
				if (CMovingBitmap::IsOverlap(cursor, menu_btns_[i])) {
					menu_btns_[i].released();

					// Remember to remove these two lines
					startSingleGame();
					return;

					switch (i) {
						case 0:
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
			}
		case placement_phase:
			if (player1_board_.ifAllShipPlaced()) {
				if (CMovingBitmap::IsOverlap(cursor, btn_start_)) {
					btn_start_.released();
					gameStart();
					// start game
				}
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
		case placement_phase:
			if (player1_board_.getCurrSel() != -1) { player1_board_.getShip().at(player1_board_.getCurrSel())->SetTopLeft(point.x - 25, point.y - 25); }
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
			for (auto& i: menu_btns_)
				i.showBtn();
			break;
		case placement_phase:
			player1_board_.show();
			if (player1_board_.ifAllShipPlaced())
				btn_start_.showBtn();
		case in_game:
			player1_board_.show();
			player2_board_.show();
		default:
			break;
	}

	// cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}

void CGameStateRun::startSingleGame() { int_phase_ = placement_phase; }

void CGameStateRun::start_mutiple_game() { int_phase_ = multiply_players; }

void CGameStateRun::gameStart() {
	// copy player1_board_ for bot's usage, and start the game Done!
	// make sure the bot's player1_board_ is shown but not its ships  Done!
	// player2_board_.whatEasesMyPainCannotBeCalledSteal(player1_board_); 
	player2_board_ = copyCatABoard(player1_board_);
	int_phase_ = in_game;
}

void CGameStateRun::player1Turn(const int& x, const int& y) {
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return;
	if (player2_board_.beingHit(x, y)) {
		if (player2_board_.ifAllShipSunk())
			int_phase_ = we_have_a_winner;
		else
			is_player1_turn_ = false;
	}
}

void CGameStateRun::player2Turn(const int& x, const int& y) {
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return;
	if (player1_board_.beingHit(x, y)) {
		if (player1_board_.ifAllShipSunk())
			int_phase_ = we_have_a_winner;
		else
			is_player1_turn_ = true;
	}
}

void CGameStateRun::gotoSettings() { int_phase_ = settings; }

void CGameStateRun::gotoExit() { GotoGameState(GAME_STATE_OVER); }
