#include "Dealer.h"



Dealer* Dealer::create(Vec2 deckPos, Vec2 gravePos)
{
	Dealer *pRet = new Dealer();
	if (pRet && pRet->init(deckPos, gravePos))
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

bool Dealer::init(Vec2 deckPos,Vec2 gravePos) 
{
	if(!Node::init())
	{
		return false;
	}

	deckSp = Sprite::create("Card/reverse.png");
	//deckSp->setScale(0.5f);
	addChild(deckSp,2);

	graveSp = Sprite::create("Card/grave.png");
	//graveSp->setScale(0.5f);
	addChild(graveSp, 0);

	setDeckPosition(deckPos,gravePos);

	setDeck(true);

	//タッチ判定
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Dealer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Dealer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Dealer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);



	return true;
};

//デッキの位置設定
void Dealer::setDeckPosition(Vec2 deckPos,Vec2 gravePos) 
{
	deckPostion = deckPos;
	deckSp->setPosition(deckPostion);
	gravePosition = gravePos;
	graveSp->setPosition(gravePosition);
};

Vec2 Dealer::getDeckPosition()
{
	return deckPostion;
}

Vec2 Dealer::getGravePosition()
{
	return gravePosition;
}

void Dealer::cardClear() 
{
	for (int i = 0; i < deck.size();i=0)
	{
		deck.at(i)->removeFromParentAndCleanup(true);
		deck.erase(i);
	}
	for (int i = 0; i < grave.size();i=0)
	{
		grave.at(i)->removeFromParentAndCleanup(true);
		grave.erase(i);
	}
}

//デッキのセット
void Dealer::setDeck(bool isJoker,int num) 
{
	cardClear();
	for (int i = MARK::SPADE; i <= MARK_MAX; i++)
	{
		for (int j = NUMBER::ONE; j <= NUMBER_MAX; j++) 
		{
			Card* card=Card::create((MARK)i, (NUMBER)j);
			//card->setScale(0.5);
			card ->setPosition(getDeckPosition());
			card->setState(STATE::DECK);
		//	card->setReverse(false);
			deck.pushBack(card);
			addChild(card);
		}
	}
	if (isJoker)
	{
		for (int i = 0; i < num; i++)
		{
			Card* joker = Card::create(MARK::JOKER_MARK,NUMBER::JOKER_NUMBER);
			//joker->setScale(0.5);
			joker->setPosition(getDeckPosition());	
			joker->setState(STATE::DECK);
		//	joker->setReverse(false);
			deck.pushBack(joker);
			addChild(joker);
		}
	}
	checkDeckZero();
};


//シャッフル
void Dealer::cardShuffle() 
{
	Vector<Card*> temp;
	for (int i = 0; deck.size();i++)
	{
		int iRand = random(0, (int)DECK_TOP);
		temp.pushBack(deck.at(iRand));
		deck.erase(iRand);
	}
	deck = temp;
};

//パーフェクトシャッフル
void Dealer::cardPerfectShuffle()
{
	Vector<Card*> temp;
	if (deck.size() == 52) {
		for (int i = 0; i < deck.size() / 2; i++)
		{
			temp.pushBack(deck.at(i));
			temp.pushBack(deck.at((i + deck.size() / 2)));
		}
		deck = temp;
	}
};

void Dealer::cardSplitShuffle(int num) 
{

}

//デッキから直接捨て札に送る
void Dealer::cardDeckThrow() 
{
	if (deck.size() > 0) 
	{
		grave.pushBack(deck.at(DECK_TOP));
		deck.at(DECK_TOP)->setState(STATE::GRAVE);
		deck.erase(DECK_TOP);
	}
};

//デッキに加える
void Dealer::cardAdd(Card* card)
{
	card->setState(STATE::DECK);	
	card->setReverse(false);
	deck.pushBack(card);
	checkDeckZero();
};

//デッキから特定のものを取り除く
void Dealer::cardLose(Card* card)
{
	for (int i = 0; i < deck.size();i++)
	{
		if (deck.at(i)==card)
		{
			deck.at(i)->setState(STATE::GRAVE);
			card->setReverse(false);
			deck.erase(i);
			return;
		}
	}
	checkDeckZero();
};

//デッキがゼロ枚かどうか
void Dealer::checkDeckZero() 
{
	if(deck.size()<=0)
	{
		deckSp->setTexture("Card/nodeck.png");
	}
	else
	{
		deckSp->setTexture("Card/reverse.png");
	}
};
//捨て札の表示
void Dealer::cardDispGrave()
{
	for (int i = 0; i < grave.size(); i++)
	{
		grave.at(i)->setPosition(graveSp->getPosition());
		grave.at(i)->setZOrder(1);
		grave.at(i)->setReverse(true);
		grave.at(i)->setColor(Color3B::WHITE);
	}
};
void Dealer::ShuffleAnimation(bool is)
{
	Animation *animation = Animation::create();
	String* name;
	for (int i = 0;i<12; i++)
	{
		name = String::createWithFormat("Card/shuffle%2d.png", i);
	animation->addSpriteFrameWithFileName(name->getCString());	
	}
	animation->setDelayPerUnit(0.3f / 4.0f);
	Animate* action = Animate::create(animation);
	//RepeatForever* anime = RepeatForever::create(action);
	deckSp->runAction(action);
}
bool Dealer::onTouchBegan(const Touch * touch, Event *unused_event) 
{

	return true;
};

void Dealer::onTouchMoved(const Touch * touch, Event *unused_event)
{

};

void Dealer::onTouchEnded(const Touch * touch, Event *unused_event)
{

};
