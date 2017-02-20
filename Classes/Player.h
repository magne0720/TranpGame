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
	void cardSort(int num);
	//ドローする
	void cardDrow(Vector<Card*>&deck);
	//カードを捨てる
	void cardThrow(int num,Vector<Card*>&grave);
	//カードすべて捨てる
	void handDeath();
	//役の調査
	void checkRole();
	//役の発見
	void chanceRole(Card* card,ROLE role,bool isAllCheck=false);
	//役の得点を計算する
	int calcRole();
	//順子
	void checkOrder(int num);
	//刻子
	void checkEqual(int num);


	Vector<Card*> hand,brain;
	//頭の中の計算
	int brainHandCount;
	//得点
	int point;
	//選んだ番号
	int pickNumber;
	//選んだパイル
	STATE pickState;

};


#endif // !__PLAYER_H__

