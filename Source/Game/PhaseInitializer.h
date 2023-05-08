#pragma once
#include "../Library/gameutil.h"

class myBtn;
class GameBoard;
using namespace game_framework;

class PhaseInitializer_base {
protected:
	const vector<vector<string>> _buttonPath = {
		{"Resources/singlePlayerButton.bmp", "Resources/singlePlayerButtonPressed.bmp"},
		{"Resources/multiplePlayerButtons.bmp", "Resources/multiplePlayerButtonPressed.bmp"},
		{"Resources/optionsButton.bmp", "Resources/optionsButtonPressed.bmp"},
		{"Resources/exitButton.bmp", "Resources/exitButtonPressed.bmp"},
		{"Resources/noobButton.bmp", "Resources/noobButtonPressed.bmp"},
		{"Resources/normalButton.bmp", "Resources/normalButtonPressed.bmp"},
		{"Resources/hardButton.bmp", "Resources/hardButtonPressed.bmp"},
		{"Resources/insaneButton.bmp", "Resources/insaneButtonPressed.bmp"}
	};
	vector<CMovingBitmap*> _container;

public:
	virtual ~PhaseInitializer_base() = default;
	virtual void init() = 0;

};

class PhaseInitializer_global final : public PhaseInitializer_base {
	const short _bgPos = 0;
	const short _cursorPos = 1;
	const short _size = 2;

public:
	explicit PhaseInitializer_global(const vector<CMovingBitmap*>& objs);//{bg, cursor}
	void init() override;
};

class PhaseInitializer_menu final : public PhaseInitializer_base {
	// std::vector<CMovingBitmap*> _buttons;
	const short _btnPos = 0;
	const short size = 4 + 4;
	vector<myBtn*> _container;

public:
	explicit PhaseInitializer_menu(const vector<myBtn*>& objs);// size = 4
	void init() override;
};

class PhaseInitializer_placement final : public PhaseInitializer_base {
	vector<myBtn*> _container;
	GameBoard* _board1 = nullptr;
	const short _startButtonPos = 0;
	const short _randomBoardButtonPos = 1;
	const short _size = 2;

public:
	PhaseInitializer_placement(GameBoard* const& board1, const vector<myBtn*>& objs);
	void init() override;
};
