#include "stdafx.h"
#include "PhaseInitializer.h"
#include "buttons.h"
#include "config.h"
#include "GameBoard.h"

#include <string>

PhaseInitializer_global::PhaseInitializer_global(const vector<CMovingBitmap*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_global::init() {
	this->_container.at(_bgPos)->LoadBitmapA(R"(Resources/Images/Backgrounds/menu.bmp)");
	this->_container.at(_bgPos)->SetTopLeft(0, 0);
	this->_container.at(_cursorPos)->LoadBitmapA(R"(Resources/Images/Misc/cursor.bmp)");
}

PhaseInitializer_menu::PhaseInitializer_menu(const vector<myBtn*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_menu::init() {
	for (int i = 0; i < 3; ++i) {
		this->_container.at(i)->LoadBitmapByString(this->_buttonPath[i]);
		this->_container.at(i)->SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * (i) + 200)));
	}

	for (int i = 3; i < 7; ++i) {
		this->_container.at(i)->LoadBitmapByString(this->_buttonPath[i]);
		this->_container.at(i)->SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * ((i + 1) % 4) + 200)));
	}
}

PhaseInitializer_placement::PhaseInitializer_placement(GameBoard* const& board1, const vector<myBtn*>& objs, const short& baseX, CMovingBitmap* const& hint) {
	this->_board1 = board1;
	this->_container = objs;
	this->_boardBaseX = baseX;
	this->hint = hint;
}

void PhaseInitializer_placement::init() {
	this->_container.at(_startButtonPos)->LoadBitmapByString({R"(Resources/Images/Buttons/Normal/gameStart.bmp)", R"(Resources/Images/Buttons/Pressed/gameStart.bmp)"});
	this->_container.at(_startButtonPos)->SetTopLeft(SIZE_X - 150 - this->_container.at(_startButtonPos)->GetWidth(), SIZE_Y - 150 - this->_container.at(_startButtonPos)->GetHeight());
	this->_container.at(_randomBoardButtonPos)->LoadBitmapByString({R"(Resources/Images/Buttons/Normal/randomBoard.bmp)", R"(Resources//Images/Buttons/Pressed/randomBoard.bmp)"});
	this->_container.at(_randomBoardButtonPos)->SetTopLeft(SIZE_X - 150 - this->_container.at(_randomBoardButtonPos)->GetWidth(), SIZE_Y - 150 - this->_container.at(_randomBoardButtonPos)->GetHeight() * 2 - 50);
	this->_board1->init(_boardBaseX);
	if (hint != nullptr) {
		this->hint->LoadBitmapA(R"(Resources/Images/Misc/hint.bmp)");
		this->hint->SetTopLeft(SIZE_X - 150 - this->hint->GetWidth(), 150);
	}
}

PhaseInitializer_ending::PhaseInitializer_ending(const vector<myBtn*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_ending::init() {
	this->_container.at(_restartButtonPos)->LoadBitmapByString({R"(Resources/Images/Buttons/Normal/restart.bmp)", R"(Resources/Images/Buttons/Pressed/restart.bmp)"});
	this->_container.at(_restartButtonPos)->SetTopLeft(SIZE_X / 2 - this->_container.at(_restartButtonPos)->GetWidth() / 2, SIZE_Y - 150 - this->_container.at(_restartButtonPos)->GetHeight() * 2 - 50);
	this->_container.at(_exitButtonPos)->LoadBitmapByString({R"(Resources/Images/Buttons/Normal/exit.bmp)", R"(Resources/Images/Buttons/Pressed/exit.bmp)"});
	this->_container.at(_exitButtonPos)->SetTopLeft(SIZE_X / 2 - this->_container.at(_exitButtonPos)->GetWidth() / 2, SIZE_Y - 150 - this->_container.at(_exitButtonPos)->GetHeight());
}

PhaseInitializer_cheatMode::PhaseInitializer_cheatMode(const vector<CMovingBitmap*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_cheatMode::init() {
	// load  bmp files named 00001.bmp to 00262 in Resources/Images/Cheat
	vector<string> paths;
	
	for (int i = 1; i <= 262; ++i) {
		string path = R"(Resources/Images/Cheat/)";
		if (i < 10) {
			path += "0000";
		}
		else if (i < 100) {
			path += "000";
		}
		else {
			path += "00";
		}
		path += to_string(i);
		path += ".bmp";
		paths.push_back(path);
	}
	this->_container.front()->LoadBitmapByString(paths, RGB(0, 0, 255));
	this->_container.front()->SetTopLeft((SIZE_X - this->_container.front()->GetWidth() )/ 2, SIZE_Y - this->_container.front()->GetHeight());
}
