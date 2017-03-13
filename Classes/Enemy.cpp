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
	ressetPlayer();
	effect = EffectManager::create();
	addChild(effect);
	setWishHand(level);
	for (int i = 0; i < 11; i++)
	{
		result.pushBack(Card::create(MARK::NOMARK, NUMBER::NONUMBER));
		result.at(i)->setRole(ROLE::WITHOUT);
		result.at(i)->roleNumber = 0;
	}
	lastCard = Card::create(MARK::NONE, NUMBER::ZERO);
	lastCard->setVisible(false);
	addChild(lastCard);
	return true;
};

//捨てるカード決める思考
void Enemy::pickCardThink()
{
	checkRole();
	pickNumber = random(0, (int)hand.size() - 1);
	while (hand.at(pickNumber)->myRole == ROLE::ROLEIN)
		pickNumber = random(0, (int)hand.size() - 1);
	if(brainEnd)isKnocked = true;
};

//役の設定
void Enemy::setWishHand(int level) 
{
	wishCard.clear();
	for (int i = 0; i < level * 2; i++)
	{
		wishCard.pushBack(Card::create((MARK)random((int)MARK::SPADE, (int)MARK::DIAMOND), (NUMBER)random((int)NUMBER::ONE, (int)NUMBER::KING)));
	}
};

//引くパイル選択
void Enemy::setDrawState(STATE state) 
{
	pickState = state;
};

//ターンによって引くパイル選択
STATE Enemy::desidePile(Card* c,int turn, bool isPass) 
{
	int iRand;
	//ほしいカードが捨て札にあれば
	for (int i = 0; i < wishCard.size(); i++)
	{
		if (wishCard.at(i)->myMark == c->myMark&&wishCard.at(i)->myNumber == c->myNumber) 
		{
			return STATE::GRAVE;
		}
	}
	//1ターン目にはパスと捨て札が選べる
	if (turn == 1) 
	{
		iRand = 1;
		//iRand = random(1, 2);
	}
	//2ターン目では選考がパスした場合にのみパスができる
	else if (turn == 2) 
	{
		if (!isPass) 
		{
			//デッキか捨て札
			iRand = random(0, 1);
		}
		else
		{
			//パスか捨て札
			iRand = random(1, 2);
		}
	}
	//そのほかの行動。デッキか捨て札
	else 
	{
		iRand = random(0, 1);
	}
	switch (iRand)
	{
	case 0:
		return STATE::DECK;
	case 1:
		return STATE::GRAVE;
	case 2:
		return STATE::FREE;
	default:
		return STATE::HAND;
	}

};

