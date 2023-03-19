#pragma once
#include "../Library/gameutil.h"

using namespace game_framework;

class PhaseManager_base {
protected:
    const vector<string> _buttonPath = {"Resources/button.bmp", "Resources/buttonPressed.bmp"};

public:
    virtual ~PhaseManager_base() = default;
    virtual void init() = 0;
    // virtual void buttonInit() = 0;

};

class PhaseManager_global final : public PhaseManager_base {
    CMovingBitmap* _bg;
    CMovingBitmap* _cursor;

public:
    explicit PhaseManager_global(const vector<CMovingBitmap*>& objs);//{bg, cursor}
    void init() override;
};

class PhaseManager_menu final : public PhaseManager_base {
    std::vector<CMovingBitmap*> _buttons;

public:
    explicit PhaseManager_menu(const initializer_list<CMovingBitmap*>& objs);// size = 4
    void init() override;

};

class PhaseManager_placement final : public PhaseManager_base {
    CMovingBitmap* _board1 = nullptr;
    CMovingBitmap* _gameStartButton = nullptr;

public:
    PhaseManager_placement(CMovingBitmap* const board1, CMovingBitmap* const gameStartButton);
    void init() override;
};
