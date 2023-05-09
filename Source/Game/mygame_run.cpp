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
#include "audioIDEnum.h"
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
	} else if (_phase == multiply_players) {
		if ((_player1Board.getSelectedShipIndex() != -1) &&
		    !_player1Board.ifAllShipPlaced()) {
			if (nChar == 52 || nChar == 82) {
				_player1Board.rotateShip();
			}
		} else if ((_player2Board.getSelectedShipIndex() != -1) &&
		           !_player2Board.ifAllShipPlaced()) {
			if (nChar == 52 || nChar == 82) {
				_player2Board.rotateShip();
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
	} else if (_phase == multiply_players || _phase == turnplay2) {
		if (_phase != turnplay2) {
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
		} else if (_phase == turnplay2) {
			if (_player2Board.getSelectedShipIndex() == -1) {
				const auto ships = _player2Board.getShip();
				for (int i = 0; i < 5; ++i) {
					// if (myIsOverlap(point, ships.at(i)) != 0) {
					if (CMovingBitmap::IsOverlap(_cursor, *ships.at(i))) {
						_player2Board.pickUpShip(i);
						break;
					}
				}
			} else {
				_player2Board.dropShip(point);
			}
			if (CMovingBitmap::IsOverlap(_cursor, _randomBoardButton)) {
				_randomBoardButton.pressed();
			}
		}
		if (_player1Board.ifAllShipPlaced() && _player2Board.ifAllShipPlaced()) {
			if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton)) {
				_gameStartButton.pressed();
			}
		}
	} else if (_phase == single_game ||
	           _phase == multi_game) {
		if (y < 0 || y > 9)
			return;
		if ((x2 < 0 || x2 > 9 && _turnFlag) && (x1 < 0 || x1 > 9 && !_turnFlag))
			return;
		bool hit = false;
		CAudio* audio = CAudio::Instance();
		if (_turnFlag) {
			hit = turn(CPoint(x2, y), 1);
			if (hit) {
				audio->Play(AudioID::player_hit);
			} else {
				audio->Play(AudioID::Missed1 + _missAudioPos);
				if (++_missAudioPos == 3)
					_missAudioPos = 0;
			}
		} else if (!_playWithRobot) {
			// if play with robot, player2Turn will be called in CGameStateRun::OnMove()
			if (hit) {
				audio->Play(AudioID::player_hit);
			} else {
				audio->Play(AudioID::Missed1 + _missAudioPos);
				if (++_missAudioPos == 3)
					_missAudioPos = 0;
			}
			hit = turn(CPoint(x1, y), 2);
		}

		_lastTimePlayerPlayed = clock();
	} else if (_phase == p1_wins || _phase == p2_wins) {
		if (CMovingBitmap::IsOverlap(_cursor, _restartButton)) {
			_restartButton.pressed();
		} else if (CMovingBitmap::IsOverlap(_cursor, _exitButton)) {
			_exitButton.pressed();
		}
	}
}

void CGameStateRun::restart() {
	// reset both board
	_player1Board.reset();
	_player2Board.reset();
	// reset phase
	_phase = menu;
	// reset turn flag
	_turnFlag = true;
	// reset the bot
	if (_playWithRobot) {
		_bot.reset();
	}
	// no need to reset button
	_playWithRobot = false;

	// reset BGM
	CAudio* audio = CAudio::Instance();
	audio->Stop(AudioID::defeated);
	audio->Stop(AudioID::defeat_not_DSMode_Bot);
	audio->Play(AudioID::theme);
	_endingThemeStarted = false;
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {
	_cursor.SetTopLeft(point.x - 5, point.y - 5);
	if (_phase == menu) {
		for (int i = 0; i < 4; ++i) {
			if (CMovingBitmap::IsOverlap(_cursor, _menuButton[i]) &&
			    _menuButton[i].GetFrameIndexOfBitmap() == 1) {
				_menuButton[i].released();
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
	} else if (_phase == multiply_players || _phase == turnplay2) {
		if (_phase != turnplay2 &&
		    CMovingBitmap::IsOverlap(_cursor, _randomBoardButton) &&
		    _randomBoardButton.GetFrameIndexOfBitmap() == 1) {
			_randomBoardButton.released();
			_player1Board = generateABoard(_player1Board.getBaseX());
		} else if (_phase == turnplay2 &&
		           CMovingBitmap::IsOverlap(_cursor, _randomBoardButton) &&
		           _randomBoardButton.GetFrameIndexOfBitmap() == 1) {
			_randomBoardButton.released();
			_player2Board = generateABoard(1020, false);
		}
		if (_phase != turnplay2) {
			if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton) &&
			    _gameStartButton.GetFrameIndexOfBitmap() == 1) {
				_gameStartButton.released();
				gameStart();
				// start game
			}
		}
		if (_player1Board.ifAllShipPlaced() && _player2Board.ifAllShipPlaced()) {
			if (CMovingBitmap::IsOverlap(_cursor, _gameStartButton) &&
			    _gameStartButton.GetFrameIndexOfBitmap() == 1) {
				_gameStartButton.released();
				gameStart();
				// start game
			}
		}
	} else if (_phase == p1_wins || _phase == p2_wins) {
		if (CMovingBitmap::IsOverlap(_cursor, _restartButton) &&
		    _restartButton.GetFrameIndexOfBitmap() == 1) {
			_restartButton.released();
			restart();
		} else if (CMovingBitmap::IsOverlap(_cursor, _exitButton) &&
		           _exitButton.GetFrameIndexOfBitmap() == 1) {
			_exitButton.released();
			GotoGameState(GAME_STATE_OVER);
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
	} else if (_phase == multiply_players) {
		if (_player1Board.getSelectedShipIndex() != -1 && !_player1Board.ifAllShipPlaced()) {
			_player1Board.getShip().at(_player1Board.getSelectedShipIndex())->SetTopLeft(point.x - 25, point.y - 25);
		} else if (_player2Board.getSelectedShipIndex() != -1) {
			_player2Board.getShip().at(_player2Board.getSelectedShipIndex())->SetTopLeft(point.x - 25, point.y - 25);
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
	} else if (_phase == multiply_players || _phase == turnplay2) {
		_randomBoardButton.showBtn();
		if (_phase != turnplay2) {
			_player1Board.show();
			if (_player1Board.ifAllShipPlaced()) {
				_gameStartButton.showBtn();
			}
		} else {
			if (_phase == turnplay2) {
				_player2Board.show();
			}
			if (_player1Board.ifAllShipPlaced() &&
			    _player2Board.ifAllShipPlaced()) {
				_gameStartButton.showBtn();
			}
		}

	} else if (_phase == single_game &&
	           _player1Board.ifAllShipPlaced() &&
	           _player2Board.ifAllShipPlaced()) {
		_player1Board.show();
		_player2Board.show();
		
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
	} else if (_phase == multiply_players || _phase == turnplay2) {
		// idk
		// _player2Board = generateABoard(1020, true);
		if (_player1Board.ifAllShipPlaced() && _player2Board.ifAllShipPlaced()) {
			_phase = multi_game;
			_player1Board.becomeEnemy();
			_player2Board.becomeEnemy();
		} else {
			_phase = turnplay2;
		}
	}
	_player1Board.setMyTurn(false);
	_player2Board.setMyTurn(true);
	// _phase = in_game;
}

bool CGameStateRun::turn(const CPoint& point, const int& player) {
	if (point.x < 0 || point.x > 9 || point.y < 0 || point.y > 9)
		return false;
	auto& board = player == 1 ? _player2Board : _player1Board;
	const int result = board.beingHit(point.x, point.y);
	if (result >= 0) {
		if (board.ifAllShipSunk()) {
			_phase = player != 1 ? p2_wins : p1_wins;
		}
		if (result != INT_MAX) {
			return true;
		}
			return false;
		}
	_player2Board.setMyTurn(player == 1);
	_player1Board.setMyTurn(player == 2);
	_turnFlag = !_turnFlag;
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
		if (clock() - _lastTimePlayerPlayed > bot_play_delay) {
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
			const bool hit = turn(pt, 2);
			CAudio* audio = CAudio::Instance();
			if (hit) {
				audio->Play(AudioID::bot_hit + _hitAudioPos.second);
				if (++_hitAudioPos.second == AudioID::hit_buffer) {
					_hitAudioPos.second = 0;
				}
			} else {
				audio->Play(AudioID::Missed1 + _missAudioPos);
				if (++_missAudioPos == 3)
					_missAudioPos = 0;
			}
			_bot.getFeedback(hit);
			_lastTimePlayerPlayed = clock();
		}
	}
	auto* audio = CAudio::Instance();
	if (_phase == p1_wins) {
		if (_playWithRobot) {
			if (! _endingThemeStarted) {
				audio->Stop(AudioID::theme);
				if (_bot.getDifficulty() != robot_enums::dark_soul) {
					audio->Play(AudioID::defeat_not_DSMode_Bot);
				}else {
					audio->Play(AudioID::defeat_dark_soul);
				}
				_endingThemeStarted = true;
			}
		}
	} else if (_phase == p2_wins) {
		audio->Stop(AudioID::theme);
		if (_playWithRobot && ! _endingThemeStarted) {
			audio->Play(AudioID::defeated);
			_endingThemeStarted = true;
		}
	}
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {
}
