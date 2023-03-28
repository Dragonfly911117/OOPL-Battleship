#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
// #include "tinyUtil.h"
#include "phaseManager.h"
using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
	: CGameState(g) {
}


void CGameStateRun::OnInit() {
	_menuButton.resize(4);
	_phase = menu;
	const vector<CMovingBitmap*> temp = {&this->_backgrounds, &this->_cursor};
	this->_phaseManagers.emplace_back(new PhaseManager_global(temp));

	vector<myBtn*> temp2 = {_menuButton.data(), &_menuButton[1], &_menuButton[2], &_menuButton[3]};
	this->_phaseManagers.emplace_back(new PhaseManager_menu(temp2));

	temp2 = {&this->_gameStartButton, &this->_randomBoardButton};
	this->_phaseManagers.emplace_back(new PhaseManager_placement(&this->_player1Board, temp2));

	for (const auto& i: this->_phaseManagers) {
		i->init();
	}

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (_phase == placement_phase) {
		if (_player1Board.getSelectedShipIndex() != -1) {
			if (nChar == 52 || nChar == 82) {
				_player1Board.rotateShip();
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == 27)
		exit(27);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {
	_cursor.SetTopLeft(point.x - 5, point.y - 5);
	const int x1 = (point.x - 150) / 60;
	const int x2 = (point.x - _player2Board.getBaseX()) / 60;
	const int y = (point.y - 150) / 60;
	if (_phase == menu) {
		for (auto& i: _menuButton) {
			if (CMovingBitmap::IsOverlap(_cursor, i)) {
				i.pressed();
				break;
			}
		}
	} else if (_phase == placement_phase) {
		if (_player1Board.getSelectedShipIndex() == -1) {
			const auto ships = _player1Board.getShip();
			for (int i = 0; i < 5; ++i) {
				// if (myIsOverlap(point, ships.at(i)) != 0) {
				if (CMovingBitmap::IsOverlap(_cursor, *ships.at(i))) {
					_player1Board.pickUpShip(i);
					break;
				}
			}
		} else {
			_player1Board.dropShip(point);
		}
		if (CMovingBitmap::IsOverlap(_cursor, _randomBoardButton)) {
			_randomBoardButton.pressed();
		}
		if (_player1Board.ifAllShipPlaced()) {
			if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton)) {
				_gameStartButton.pressed();
			}
		}
	} else if (_phase == in_game) {
		if (y < 0 || y > 9)
			return;
		if ((x2 < 0 || x2 > 9 && _turnFlag) && (x1 < 0 || x1 > 9 && !_turnFlag))
			return;
		_turnFlag ? player1Turn(x2, y) : player2Turn(x1, y);
	} else {
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
	_cursor.SetTopLeft(point.x - 5, point.y - 5);
	if (_phase == menu) {
		// Remember to remove these two lines
		// startSingleGame();
		// return;
		for (int i = 0; i < 4; ++i) {
			if (CMovingBitmap::IsOverlap(_cursor, _menuButton[i]) &&
			    _menuButton[i].GetFrameIndexOfBitmap() == 1) {
				if (i == 0) {
					startSingleGame();
				} else if (i == 1) {
					start_mutiple_game();
				} else if (i == 2) {
					gotoSettings();
				} else if (i == 3) {
					gotoExit();
				}
			}
		}
	} else if (_phase == placement_phase) {
		if (CMovingBitmap::IsOverlap(_cursor, _randomBoardButton) &&
		    _randomBoardButton.GetFrameIndexOfBitmap() == 1) {
			_randomBoardButton.released();
			_player1Board = generateABoard(_player1Board.getBaseX());
		}
		if (_player1Board.ifAllShipPlaced()) {
			if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton) &&
			    _gameStartButton.GetFrameIndexOfBitmap() == 1) {
				_gameStartButton.released();
				gameStart();
				// start game
			}
		}
	} else {

	}

}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {
	_cursor.SetTopLeft(point.x - 5, point.y - 5);
	if (_phase == menu) {
		// copilot suggested code, add this function in the future
		// for (auto& i : menu_btns) {
		//     if (CMovingBitmap::IsOverlap(cursor, i)) {
		//         i.hover();
		//         break;
		//     }
		//     else if (i.GetFrameIndexOfBitmap() == 1)
		//         i.released();
		// }
		// for (auto& i: _menuButton) {
		//     if (CMovingBitmap::IsOverlap(_cursor, i)) {
		//         break;
		//     }
		//     i.released();
		// }
	} else if (_phase == placement_phase) {
		if (_player1Board.getSelectedShipIndex() != -1) {
			_player1Board.getShip().at(_player1Board.getSelectedShipIndex())->SetTopLeft(point.x - 25, point.y - 25);
		}
	} else {
	}
}

void CGameStateRun::OnShow() {
	_backgrounds.ShowBitmap();
	if (_phase == menu) {
		_phaseManagers.at(1)->show();
	} else if (_phase == placement_phase) {
		_phaseManagers.at(2)->show();
	} else if (_phase == in_game) {
		// Perhaps consider making a new PhaseManager for this one.
		_player1Board.show();
		_player2Board.show();
	} else {
	}
	// cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}

void CGameStateRun::startSingleGame() {
	_phase = placement_phase;
}

void CGameStateRun::start_mutiple_game() {
	_phase = multiply_players;
}

void CGameStateRun::gameStart() {
	// _player2Board = copyABoard(_player1Board);// can have
	_player2Board = generateABoard(1020);
	_phase = in_game;
}

void CGameStateRun::player1Turn(const int& x, const int& y) {
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return;
	if (_player2Board.beingHit(x, y) >= 0) {
		if (_player2Board.ifAllShipSunk()) {
			_phase = we_have_a_winner;
		}
		return;
	}
	_turnFlag = false;
}

void CGameStateRun::player2Turn(const int& x, const int& y) {
	if (x < 0 || x > 9 || y < 0 || y > 9)
		return;
	if (_player1Board.beingHit(x, y) >= 0) {
		if (_player1Board.ifAllShipSunk()) {
			_phase = we_have_a_winner;
		}
		return;
	}
	_turnFlag = true;
}

void CGameStateRun::gotoSettings() {
	_phase = settings;
}

void CGameStateRun::gotoExit() {
	GotoGameState(GAME_STATE_OVER);
}

// unused functions

void CGameStateRun::OnBeginState() {
}

void CGameStateRun::OnMove() {
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {
}
