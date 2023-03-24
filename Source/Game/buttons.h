#pragma once
#pragma warning (disable: 4018) // fk signed/unsigned mismatch

using namespace game_framework;

class myBtn : public CMovingBitmap {
protected:
	std::string _text;

public:
	myBtn();
	void setText(const string& str);
	void pressed();
	void released();
	void showBtn();
};
