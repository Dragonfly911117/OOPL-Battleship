#pragma once
#pragma warning (disable: 4018) // fk signed/unsigned mismatch
#include <unordered_set>

using namespace game_framework;

class myBtn : public CMovingBitmap {
protected:
    std::string text;

public:
    myBtn();
    void setText(const string& str);
    void pressed();
    void released();
    void showBtn();
};

void buttonsInit(myBtn menuBtn[4], myBtn& startButton);
