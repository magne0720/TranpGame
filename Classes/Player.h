#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

//プレイヤー
class Player :public Node 
{
public:
	CREATE_FUNC(Player);
	bool init();
	//カードの表示
	void cardDispHand();
	//カードの並び替え
	void cardSort(ROLE kind);
	//ドローする
	void cardDrow(Vector<Card*>&deck);
	//カードを捨てる
	void cardThrow(int num,Vector<Card*>&grave);
	//カードすべて捨てる
	void handDeath();
	//役の初期化
	void ressetRole();
	//役の調査
	int checkRole();
	//役の発見
	void chanceRole(Card* card,ROLE role,bool isAllCheck=false);
	//役の得点を計算する
	int calcRole();
	//順子
	void checkOrder(int num);
	//刻子
	void checkEqual(int num);
	//三枚のところからカードを抜いていいかどうか
	void checkOrderExcept();
	//三枚のところからカードを抜いていいかどうか
	void checkEqualExcept();
	//四枚刻子の一枚抜きOK検査
	void checkEqualFour(Card* card);
	//四枚順子の一枚抜きOK検査
	void checkOrderFour(Card* card);

	Vector<Card*> hand,brain;
	//頭の中の計算
	int brainHandCount;
	//得点
	int point;
	//選んだ番号
	int pickNumber;
	//選んだパイル
	STATE pickState;
	//並び替えの状態
	ROLE sortType;

};


#endif // !__PLAYER_H__

