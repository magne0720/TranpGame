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
	//カードの表示
	void cardDispHand();
	//カードを捨てる
	void cardThrow(Card* &card);
	//カードすべて捨てる
	void handDeath();
	//役を見る
	void checkFame(int x);
	//順子
	void checkOrder(int num);


	Vector<Card*> hand;
	int point;

};


#endif // !__PLAYER_H__

