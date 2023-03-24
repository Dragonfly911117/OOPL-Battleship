#pragma once
#include "../Library/gameutil.h"

class myBtn;
class GameBoard;
using namespace game_framework;

class PhaseManager_base {
protected:
	const vector<string> _buttonPath = {"Resources/button.bmp", "Resources/buttonPressed.bmp"};
	vector<CMovingBitmap*> _container;

public:
	virtual ~PhaseManager_base() = default;
	virtual void init() = 0;
	virtual void show() = 0;
	// virtual void buttonInit() = 0;

};

class PhaseManager_global final : public PhaseManager_base {
	const short _bgPos = 0;
	const short _cursorPos = 1;
	const short _size = 2;

public:
	explicit PhaseManager_global(const vector<CMovingBitmap*>& objs);//{bg, cursor}
	void init() override;
	void show() override;
};

class PhaseManager_menu final : public PhaseManager_base {
	// std::vector<CMovingBitmap*> _buttons;
	const short _btnPos = 0;
	const short size = 4;
	vector<myBtn*> _container;

public:
	explicit PhaseManager_menu(const vector<myBtn*>& objs);// size = 4
	void init() override;
	void show() override;

};

class PhaseManager_placement final : public PhaseManager_base {
	vector<myBtn*> _container;
	GameBoard* _board1 = nullptr;
	const short _startButtonPos = 0;
	const short _randomBoardButtonPos = 1;
	const short _size = 2;
public:
	PhaseManager_placement(GameBoard* const& board1, const vector<myBtn*>& objs);
	void init() override;
	void show() override;
};
