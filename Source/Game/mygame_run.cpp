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
#include "PhaseInitializer.h"
#include  "Robot.h"
using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
	: CGameState(g) {
}

void CGameStateRun::OnInit() {
	_menuButton.resize(4);
	_difficultyButton.resize(4);
	_phase = menu;
	vector<shared_ptr<PhaseInitializer_base>> initializer;
	const vector<CMovingBitmap*> temp = {&this->_background, &this->_cursor};
	initializer.emplace_back(new PhaseInitializer_global(temp));

	vector<myBtn*> temp2 = {&_menuButton[0], &_menuButton[1], &_menuButton[2], &_menuButton[3],
	                        &_difficultyButton[0], &_difficultyButton[1], &_difficultyButton[2], &_difficultyButton[3]};
	initializer.emplace_back(new PhaseInitializer_menu(temp2));

	temp2 = {&this->_gameStartButton, &this->_randomBoardButton};
	initializer.emplace_back(new PhaseInitializer_placement(&this->_player1Board, temp2));

	temp2 = {&this->_restartButton, &this->_exitButton};
	initializer.emplace_back(new PhaseInitializer_ending(temp2));
	for (const auto& i: initializer) {
		i->init();
	}

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (_phase == single_placement_phase) {
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

	} else if (_phase == difficulty_choosing) {
		for (auto& i: _difficultyButton) {
			if (CMovingBitmap::IsOverlap(_cursor, i)) {
				i.pressed();
				break;
			}
		}
	} else if (_phase == single_placement_phase) {
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
	} else if (_phase == single_game) {
		if (y < 0 || y > 9)
			return;
		if ((x2 < 0 || x2 > 9 && _turnFlag) && (x1 < 0 || x1 > 9 && !_turnFlag))
			return;
		if (_turnFlag) {
			turn(CPoint(x2, y), 1);
		} else if (!_playWithRobot) {
			// if play with robot, player2Turn will be called in CGameStateRun::OnMove()
			turn(CPoint(x1, y), 2);
		}
	} else {
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
	_cursor.SetTopLeft(point.x - 5, point.y - 5);
	if (_phase == menu) {
		for (int i = 0; i < 4; ++i) {
			if (CMovingBitmap::IsOverlap(_cursor, _menuButton[i]) &&
			    _menuButton[i].GetFrameIndexOfBitmap() == 1) {
				if (i == 0) {
					startSingleGame();
				} else if (i == 1) {
					startMultiplePlayersGame();
				} else if (i == 2) {
					gotoSettings();
				} else if (i == 3) {
					gotoExit();
				}
			}
		}
	} else if (_phase == difficulty_choosing) {
		for (int i = 0; i < _difficultyButton.size(); ++i) {
			if (CMovingBitmap::IsOverlap(_cursor, _difficultyButton[i]) &&
			    _difficultyButton[i].GetFrameIndexOfBitmap() == 1) {
				_difficultyButton[i].released();
				_bot.setDifficulty(i);
				break;
			}
		}
		_phase = single_placement_phase;
	} else if (_phase == single_placement_phase) {
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
	} else if (_phase == single_placement_phase) {
		if (_player1Board.getSelectedShipIndex() != -1) {
			_player1Board.getShip().at(_player1Board.getSelectedShipIndex())->SetTopLeft(point.x - 25, point.y - 25);
		}
	} else {
	}
}

void CGameStateRun::OnShow() {
	_background.ShowBitmap();
	if (_phase == menu) {
		for (auto& i: _menuButton) {
			i.showBtn();
		}
	} else if (_phase == difficulty_choosing) {
		for (auto& i: _difficultyButton) {
			i.showBtn();
		}
	} else if (_phase == single_placement_phase) {
		_player1Board.show();
		_randomBoardButton.showBtn();
		if (_player1Board.ifAllShipPlaced()) {
			_gameStartButton.showBtn();
		}
	} else if (_phase == single_game || _phase == multiply_players) {
		if (_player2Board.ifAllCMovingBitmapLoaded() && _player1Board.ifAllCMovingBitmapLoaded()) {
			_player1Board.show();
			_player2Board.show();
		}
	} else if (_phase == p1_wins || _phase == p2_wins) {
		_restartButton.showBtn();
		_exitButton.showBtn();

	} else {
	}
	// cursor.ShowBitmap(); // un-comment this to see where the cursor is 
}

void CGameStateRun::startSingleGame() {
	_phase = difficulty_choosing;
	_playWithRobot = true;
}

void CGameStateRun::startMultiplePlayersGame() {
	_phase = multiply_players;
}

void CGameStateRun::gameStart() {
	// _player2Board = copyABoard(_player1Board);// can have
	if (_phase == single_placement_phase) {
		if (_bot.getDifficulty() == robot_enums::dark_soul) {
			_bot.gatherEnemyShipCoordinates(getShipCoordinates(_player1Board));
		}
		_player2Board = generateABoard(1020, true);
		_phase = single_game;
	} else if (_phase == multiply_players) {
		// idk
	} else {

	}
	// _phase = in_game;
}

bool CGameStateRun::turn(const CPoint& point, const int& player) {
	if (point.x < 0 || point.x > 9 || point.y < 0 || point.y > 9)
		return false;
	auto& board = player == 1 ? _player2Board : _player1Board;
	const int result = board.beingHit(point.x, point.y);
	if (result >= 0) {
		if (board.ifAllShipSunk()) {
			_phase = player == 1 ? p2_wins : p1_wins;
		}
		if (result != INT_MAX) {
			_lastTimePlayerPlayed = clock();
			return true;
		} else {
			_lastTimePlayerPlayed = clock();
			return false;
		}
	}
	_player2Board.setMyTurn(player == 1);
	_player1Board.setMyTurn(player == 2);
	_turnFlag = !_turnFlag;
	_lastTimePlayerPlayed = clock();
	return false;
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
	if (_playWithRobot && !_turnFlag) {
		if (clock() - _lastTimePlayerPlayed > _botPlayDelay) {
			CPoint pt;
			if (_bot.getDifficulty() == robot_enums::infinite_monkey) {
				pt = _bot.infiniteMonkeyModeFire();
			} else if (_bot.getDifficulty() == robot_enums::normal) {
				pt = _bot.normalModeFire();
			} else if (_bot.getDifficulty() == robot_enums::hard) {
				pt = _bot.hardModeFire();
			} else if (_bot.getDifficulty() == robot_enums::dark_soul) {
				pt = _bot.darkSoulModeFire();
			}
			_bot.getFeedback(turn(pt, 2));
			_lastTimePlayerPlayed = clock();
		}
	}
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {
}
