#pragma once
#pragma warning (disable: 4018) // fk signed/unsigned mismatch

using namespace game_framework;

class myBtn : public CMovingBitmap {
protected:

public:
	myBtn();
	void pressed();
	void released();
	void showBtn();
};
