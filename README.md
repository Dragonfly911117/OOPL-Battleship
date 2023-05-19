# 物件導向程式設計實習:海戰棋

## 規則
* 擺放階段
  * 使用滑鼠點擊以選取船隻，將船隻放置在棋盤上任意位置（不可重疊）
  * 按下`R` （或`r`）鍵可旋轉船隻
  * 或按下`Random Board`按鈕電腦自動擺放
  * 可選取已在棋盤上之船隻，並調整至新位置擺放
  * 確認好陣型後按下`Game Start`按鍵開始遊戲（雙人模式則是輪到玩家二擺放）
* 回合進行
  * 玩家一先行，選擇棋盤上一格進行猜測
  * 若猜中敵方戰艦（以![擊中](Resources/Images/Misc/UsedByREADME/shipHit.jpg)標示）則繼續猜測，否則（以![藍色酷方框](Resources/Images/Misc/UsedByREADME/gridHit.jpg)標示）輪到玩家二
  * 玩家二猜測完後輪到玩家一
  * 當一方所有戰艦被擊沉時，遊戲結束
  * 輪到玩家一時，右邊棋盤背景以 <font color="#BB7A4A">棕色</font> 標示，輪到玩家二時則以 <font color="#4E4E4E">灰色</font> 標示。反之亦然

