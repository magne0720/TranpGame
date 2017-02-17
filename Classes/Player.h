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
	void update(float delta);
	//カードの並び替え
	void cardDispHand();
	//カードの表示
	void cardSort(int num);
	//カードを捨てる
	Card* cardThrow(int num);
	//カードすべて捨てる
	void handDeath();
	//役の調査
	void checkRole(int i=0);
	//役の発見
	void chanceRole(Card* card,ROLE role,bool isAllCheck=false);
	//順子
	void checkOrder(int num);


	Vector<Card*> hand,brain;
	int brainHandCount;
	int point;

};


#endif // !__PLAYER_H__

