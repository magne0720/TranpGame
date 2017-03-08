#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class Enemy:public Player
{
public :

	static  Enemy* create(int level);
	bool init(int level);
	
	int level;
	//捨てるカード思考
	int pickCardThink();
	//引くパイル選択
	STATE setDrawState();
	//




};


#endif // !__ENEMY_H__
