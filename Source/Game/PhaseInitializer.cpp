﻿#include "stdafx.h"
#include "PhaseInitializer.h"
#include "buttons.h"
#include "config.h"
#include "GameBoard.h"

PhaseInitializer_global::PhaseInitializer_global(const vector<CMovingBitmap*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_global::init() {
	this->_container.at(_bgPos)->LoadBitmapA("Resources/menuBg.bmp");
	this->_container.at(_bgPos)->SetTopLeft(0, 0);
	this->_container.at(_cursorPos)->LoadBitmapA("Resources/cursor.bmp");
}


PhaseInitializer_menu::PhaseInitializer_menu(const vector<myBtn*>& objs) {
	this->_container = objs;
}

void PhaseInitializer_menu::init() {
	const vector<string> menuText = {"Single Player", "Multiple Players", "Options", "Exit", "Noob", "Normal", "Hard", "Insane"};
	for (int i = 0; i < menuText.size(); ++i) {
		this->_container.at(i)->setText(menuText.at(i));
		this->_container.at(i)->LoadBitmapByString(this->_buttonPath);
		this->_container.at(i)->SetTopLeft(static_cast<int>((SIZE_X * 0.4)), static_cast<int>((SIZE_Y * 0.2 * (i % 4) + 200)));
	}
}

PhaseInitializer_placement::PhaseInitializer_placement(GameBoard* const& board1, const vector<myBtn*>& objs) {
	this->_board1 = board1;
	this->_container = objs;
}

void PhaseInitializer_placement::init() {
	this->_container.at(_startButtonPos)->setText("Game Start!");
	this->_container.at(_startButtonPos)->LoadBitmapByString(_buttonPath);
	this->_container.at(_startButtonPos)->SetTopLeft(SIZE_X - 150 - this->_container.at(_startButtonPos)->GetWidth(), SIZE_Y - 150 - this->_container.at(_startButtonPos)->GetHeight());
	this->_container.at(_randomBoardButtonPos)->setText("Random Board");
	this->_container.at(_randomBoardButtonPos)->LoadBitmapByString(_buttonPath);
	this->_container.at(_randomBoardButtonPos)->SetTopLeft(SIZE_X - 150 - this->_container.at(_randomBoardButtonPos)->GetWidth(), SIZE_Y - 150 - this->_container.at(_randomBoardButtonPos)->GetHeight() * 2 - 50);
	this->_board1->init();
}