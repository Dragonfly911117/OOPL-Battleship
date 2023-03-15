#include "mygame.h"
using namespace game_framework;

class gameBoard : public CMovingBitmap {
    friend gameBoard copyABoard(const gameBoard& copied);
    // Since setup pos before  CGameStateRun::OnInit() crashes the game Constructors are NOT used 
    std::vector<vector<BaseGrid*>> grids_;
    vector<Ship*> ships_;
    vector<BaseGrid*> ship_hit_;
    int currently_sel_ship_ = -1;
    int base_x_ = 0;
    int base_y_ = 0;
    bool is_enemy_ = false;
    
public:
    // shared methods & variables
    vector<Ship*> getShip();
    void show();

    // deployment-phase methods
    void init();
    int getCurrSel() const;
    void pickUpShip(const int& sel);
    void rotateShip();
    void dropShip(const CPoint& pt);
    bool ifAllShipPlaced() const;
    void gettingStart();
    void whatEasesMyPainCannotBeCalledSteal(const gameBoard& copied);

    // main-game-phase methods
    bool beingHit(const int& x, const int& y);
    bool ifAllShipSunk() const;
};
