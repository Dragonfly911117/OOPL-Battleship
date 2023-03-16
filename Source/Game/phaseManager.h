#pragma once
#include "mygame.h"

namespace game_framework {
    class CMovingBitmap;
}

using namespace  game_framework;


class PhaseManager_base {
public:
    virtual ~PhaseManager_base() = default;
    virtual  void init() = 0;

};
class PhaseManager_global final : public  PhaseManager_base {
    CMovingBitmap* _background = nullptr;
    CMovingBitmap* _cursor = nullptr;
    
public:
    explicit PhaseManager_global(const initializer_list<CMovingBitmap*>& objs); // size = 1+1
    void init() override;
};

class PhaseManager_menu final : public PhaseManager_base {
    std::vector<myBtn*> _buttons;
public:
    explicit PhaseManager_menu(const initializer_list<myBtn*>& buttons); // size = 4
    void init() override;
    
}; 

class PhaseManager_placement final : public PhaseManager_base {
    GameBoard* _board1 = nullptr;
    myBtn* _gameStartButton = nullptr;
public:
    
    PhaseManager_placement(GameBoard* const board1, myBtn* const gameStartButton); 
    void init() override;
};
