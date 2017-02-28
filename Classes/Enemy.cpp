#include "Enemy.h"

Enemy* Enemy::create(int level) 
{
	Enemy *pRet = new Enemy();
	if (pRet && pRet->init(level))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
};

bool Enemy::init(int level) 
{
	if (!Node::init()) 
	{
	return false;
	}
	return true;
};

int Enemy::pickCardThink() 
{
	return 0;
};
