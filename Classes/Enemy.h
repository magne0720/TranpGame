#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;


//元々作る役を決めておく
//ほしいカードが捨て札にあるかどうか調べて、いるならもらう
//必要のないカードを適当に捨てる
//レベルによってノック速度を変える
class Enemy:public Player
{
public :

	//自身がほしいカード群
	Vector<Card*> wishCard;

	static  Enemy* create(int level);
	bool init(int level);
	
	//レベル
	int level;
	//ノックするターン数
	int knockTurn;
	
	//役の設定
	void setWishHand(int level);
	//捨てるカード思考
	void pickCardThink();
	//引くパイル選択
	void setDrawState(STATE state);
	//ターンによって引くパイル選択
	STATE desidePile(Card* c,int turn,bool isPass=false);




};


#endif // !__ENEMY_H__
