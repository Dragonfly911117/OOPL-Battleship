#pragma once
#include "../Library/gameutil.h"

class myBtn;
class GameBoard;
using namespace game_framework;

class PhaseInitializer_base {
protected:
	const vector<vector<string>> _buttonPath = {
		{R"(Resources/Images/Buttons/Normal/singlePlayer.bmp)", R"(Resources/Images/Buttons/Pressed/singlePlayer.bmp)"},
		{R"(Resources/Images/Buttons/Normal/multiplePlayers.bmp)", R"(Resources/Images/Buttons/Pressed/multiplePlayers.bmp)"},
		{R"(Resources/Images/Buttons/Normal/exit.bmp)", "Resources/Images/Buttons/Pressed/exit.bmp"},

		{R"(Resources/Images/Buttons/Normal/noob.bmp)", R"(Resources/Images/Buttons/Pressed/noob.bmp)"},
		{R"(Resources/Images/Buttons/Normal/normal.bmp)", R"(Resources/Images/Buttons/Pressed/normal.bmp)"},
		{R"(Resources/Images/Buttons/Normal/hard.bmp)", R"(Resources/Images/Buttons/Pressed/hard.bmp)"},
		{R"(Resources/Images/Buttons/Normal/insane.bmp)", R"(Resources/Images/Buttons/Pressed/insane.bmp)"}
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
	short _boardBaseX;
	
public:
	PhaseInitializer_placement(GameBoard* const& board1, const vector<myBtn*>& objs, const short& baseX);
	void init() override;
};

class PhaseInitializer_ending final : public PhaseInitializer_base {
	vector<myBtn*> _container;
	const short _restartButtonPos = 0;
	const short _exitButtonPos = 1;
public:
	PhaseInitializer_ending(const vector<myBtn*>& objs);
	void init() override;
};
