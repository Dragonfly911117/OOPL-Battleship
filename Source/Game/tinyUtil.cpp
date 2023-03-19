#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>

#include "config.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "tinyUtil.h"

#include <time.h>

// return the block index if overlap, return 0 if not overlap, return -1 if overlap but not on a block.
int myIsOverlap(const CPoint& pt1, Ship* ship) {
    auto lt = CPoint(ship->GetLeft(), ship->GetTop());
    CPoint rb;// dont use GetHeight()  and GetWidth(). They are not the same as the size of the bitmap after rotations.
    int direction = ship->GetFrameIndexOfBitmap();
    rb = direction == 0
             ? CPoint(lt.x + ship->getSize() * 60 - 10, lt.y + 50)
             : CPoint(lt.x + 50, lt.y + ship->getSize() * 60 - 10);
    if (pt1.x > lt.x && pt1.x < rb.x && pt1.y > lt.y && pt1.y < rb.y) {
        // which block exactly is clicked
        for (int i = 1; i <= ship->getSize(); ++i) {
            CPoint pt2 = direction == 0
                             ? CPoint(lt.x + i * 60 - 10, lt.y + 50)
                             : CPoint(lt.x + 50, lt.y + i * 60 - 10);
            if (pt1.x > pt2.x - 50 && pt1.x < pt2.x && pt1.y > pt2.y - 50 && pt1.y < pt2.y) { return i; }
        }
        return -1;
    }
    return 0;
}

int** CreateShipID() {

	int*** ShipID = new int**[10];
	for (int i = 0; i < 10; ++i) {
		ShipID[i] = new int*[10];
		for (int j = 0; j < 10; ++j) {
			ShipID[i][j] = new int[10];
			for (int k = 0; k < 10; k++)
			{
			ShipID[i][j][k] = -1;
			}
		}
	}

	ShipID[0][0][2] = 0;
	ShipID[0][1][2] = 0;
	ShipID[0][7][1] = 1;
	ShipID[0][8][1] = 1;
	ShipID[0][9][1] = 1;
	ShipID[0][0][5] = 2;
	ShipID[0][0][6] = 2;
	ShipID[0][0][7] = 2;
	ShipID[0][8][4] = 3;
	ShipID[0][8][5] = 3;
	ShipID[0][8][6] = 3;
	ShipID[0][8][7] = 3;
	ShipID[0][3][9] = 4;
	ShipID[0][4][9] = 4;
	ShipID[0][5][9] = 4;
	ShipID[0][6][9] = 4;
	ShipID[0][7][9] = 4;

	ShipID[1][0][2] = 0;
	ShipID[1][1][2] = 0;
	ShipID[1][7][1] = 1;
	ShipID[1][8][1] = 1;
	ShipID[1][9][1] = 1;
	ShipID[1][0][5] = 2;
	ShipID[1][0][6] = 2;
	ShipID[1][0][7] = 2;
	ShipID[1][8][4] = 3;
	ShipID[1][8][5] = 3;
	ShipID[1][8][6] = 3;
	ShipID[1][8][7] = 3;
	ShipID[1][3][9] = 4;
	ShipID[1][4][9] = 4;
	ShipID[1][5][9] = 4;
	ShipID[1][6][9] = 4;
	ShipID[1][7][9] = 4;

	ShipID[2][0][2] = 0;
	ShipID[2][1][2] = 0;
	ShipID[2][7][1] = 1;
	ShipID[2][8][1] = 1;
	ShipID[2][9][1] = 1;
	ShipID[2][0][5] = 2;
	ShipID[2][0][6] = 2;
	ShipID[2][0][7] = 2;
	ShipID[2][8][4] = 3;
	ShipID[2][8][5] = 3;
	ShipID[2][8][6] = 3;
	ShipID[2][8][7] = 3;
	ShipID[2][3][9] = 4;
	ShipID[2][4][9] = 4;
	ShipID[2][5][9] = 4;
	ShipID[2][6][9] = 4;
	ShipID[2][7][9] = 4;

	ShipID[3][0][2] = 0;
	ShipID[3][1][2] = 0;
	ShipID[3][7][1] = 1;
	ShipID[3][8][1] = 1;
	ShipID[3][9][1] = 1;
	ShipID[3][0][5] = 2;
	ShipID[3][0][6] = 2;
	ShipID[3][0][7] = 2;
	ShipID[3][8][4] = 3;
	ShipID[3][8][5] = 3;
	ShipID[3][8][6] = 3;
	ShipID[3][8][7] = 3;
	ShipID[3][3][9] = 4;
	ShipID[3][4][9] = 4;
	ShipID[3][5][9] = 4;
	ShipID[3][6][9] = 4;
	ShipID[3][7][9] = 4;

	ShipID[4][0][2] = 0;
	ShipID[4][1][2] = 0;
	ShipID[4][7][1] = 1;
	ShipID[4][8][1] = 1;
	ShipID[4][9][1] = 1;
	ShipID[4][0][5] = 2;
	ShipID[4][0][6] = 2;
	ShipID[4][0][7] = 2;
	ShipID[4][8][4] = 3;
	ShipID[4][8][5] = 3;
	ShipID[4][8][6] = 3;
	ShipID[4][8][7] = 3;
	ShipID[4][3][9] = 4;
	ShipID[4][4][9] = 4;
	ShipID[4][5][9] = 4;
	ShipID[4][6][9] = 4;
	ShipID[4][7][9] = 4;

	ShipID[5][0][2] = 0;
	ShipID[5][1][2] = 0;
	ShipID[5][7][1] = 1;
	ShipID[5][8][1] = 1;
	ShipID[5][9][1] = 1;
	ShipID[5][0][5] = 2;
	ShipID[5][0][6] = 2;
	ShipID[5][0][7] = 2;
	ShipID[5][8][4] = 3;
	ShipID[5][8][5] = 3;
	ShipID[5][8][6] = 3;
	ShipID[5][8][7] = 3;
	ShipID[5][3][9] = 4;
	ShipID[5][4][9] = 4;
	ShipID[5][5][9] = 4;
	ShipID[5][6][9] = 4;
	ShipID[5][7][9] = 4;

	ShipID[6][0][2] = 0;
	ShipID[6][1][2] = 0;
	ShipID[6][7][1] = 1;
	ShipID[6][8][1] = 1;
	ShipID[6][9][1] = 1;
	ShipID[6][0][5] = 2;
	ShipID[6][0][6] = 2;
	ShipID[6][0][7] = 2;
	ShipID[6][8][4] = 3;
	ShipID[6][8][5] = 3;
	ShipID[6][8][6] = 3;
	ShipID[6][8][7] = 3;
	ShipID[6][3][9] = 4;
	ShipID[6][4][9] = 4;
	ShipID[6][5][9] = 4;
	ShipID[6][6][9] = 4;
	ShipID[6][7][9] = 4;

	ShipID[7][0][2] = 0;
	ShipID[7][1][2] = 0;
	ShipID[7][7][1] = 1;
	ShipID[7][8][1] = 1;
	ShipID[7][9][1] = 1;
	ShipID[7][0][5] = 2;
	ShipID[7][0][6] = 2;
	ShipID[7][0][7] = 2;
	ShipID[7][8][4] = 3;
	ShipID[7][8][5] = 3;
	ShipID[7][8][6] = 3;
	ShipID[7][8][7] = 3;
	ShipID[7][3][9] = 4;
	ShipID[7][4][9] = 4;
	ShipID[7][5][9] = 4;
	ShipID[7][6][9] = 4;
	ShipID[7][7][9] = 4;

	ShipID[8][0][2] = 0;
	ShipID[8][1][2] = 0;
	ShipID[8][7][1] = 1;
	ShipID[8][8][1] = 1;
	ShipID[8][9][1] = 1;
	ShipID[8][0][5] = 2;
	ShipID[8][0][6] = 2;
	ShipID[8][0][7] = 2;
	ShipID[8][8][4] = 3;
	ShipID[8][8][5] = 3;
	ShipID[8][8][6] = 3;
	ShipID[8][8][7] = 3;
	ShipID[8][3][9] = 4;
	ShipID[8][4][9] = 4;
	ShipID[8][5][9] = 4;
	ShipID[8][6][9] = 4;
	ShipID[8][7][9] = 4;

	ShipID[9][0][2] = 0;
	ShipID[9][1][2] = 0;
	ShipID[9][7][1] = 1;
	ShipID[9][8][1] = 1;
	ShipID[9][9][1] = 1;
	ShipID[9][0][5] = 2;
	ShipID[9][0][6] = 2;
	ShipID[9][0][7] = 2;
	ShipID[9][8][4] = 3;
	ShipID[9][8][5] = 3;
	ShipID[9][8][6] = 3;
	ShipID[9][8][7] = 3;
	ShipID[9][3][9] = 4;
	ShipID[9][4][9] = 4;
	ShipID[9][5][9] = 4;
	ShipID[9][6][9] = 4;
	ShipID[9][7][9] = 4;

	srand((unsigned int)time(NULL));
	int randomNum = rand() % 10;
	return ShipID[randomNum];
}



GameBoard copyABoard(const GameBoard& copied) {
	// 建立一個新的 GameBoard 實例
	GameBoard newBoard;

	// 複製 copied 物件的底座位置及是否為敵方的屬性值
	newBoard.base_x_ = copied.base_x_;
	newBoard.base_y_ = copied.base_y_;
	newBoard.is_enemy_ = true;
	
	// 宣告一個字串向量，表示空白網格和擊中網格的圖像檔案路徑
	vector<string> fileName = { "Resources/emptyGrid.bmp", "Resources/gridHit.bmp" };

	// 建立一個 10x10 的迴圈，用於複製 copied 物件的網格
	for (int i = 0; i < 10; ++i) {
		vector<BaseGrid*> curr;
		for (int j = 0; j < 10; ++j) {
			// 建立一個新的空白網格
			auto newGrid = new EmptyGrid;

			// 讀取空白網格和擊中網格的圖像檔案，並設置網格的船艦 ID 和位置
			newGrid->LoadBitmapByString(fileName);
			//newGrid->setShipID(copied.grids_.at(i).at(j)->getShipID());
			int** NewShipIDArray = CreateShipID();
			newGrid->setShipID(NewShipIDArray[i][j]);
			newGrid->SetTopLeft(copied.grids_.at(i).at(j)->GetLeft() + 1020, copied.grids_.at(i).at(j)->GetTop());
			curr.push_back(newGrid);
		}
		// 將新的網格向量添加到新的 GameBoard 實例中
		newBoard.grids_.push_back(curr);
	}

	// 迴圈複製 copied 物件的所有船艦，並添加到新的 GameBoard 實例中
	for (auto& ship : copied.ships_) {
		newBoard.ships_.push_back(copyAShip(ship));
	}

	// 返回新的 GameBoard 實例
	return newBoard;
}



