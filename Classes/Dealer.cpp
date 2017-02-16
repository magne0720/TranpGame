#include "Dealer.h"



Dealer* Dealer::create(Vec2 deckPos)
{
	Dealer *pRet = new Dealer();
	if (pRet && pRet->init(deckPos))
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

bool Dealer::init(Vec2 deckPos) 
{
	if(!Node::init())
	{
		return false;
	}

	deckSp = Sprite::create("Card/reverse.png");
	deckSp->setScale(0.5f);
	setDeckPosition(deckPos);
	addChild(deckSp,1);

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
void Dealer::setDeckPosition(Vec2 pos) 
{
	deckPostion = pos;
	deckSp->setPosition(deckPostion);
};

Vec2 Dealer::getDeckPosition() 
{
	return deckPostion;
}

void Dealer::cardClear() 
{
	for (int i = 0; i < deck.size();i=0)
	{
		deck.at(i)->removeFromParentAndCleanup(true);
		deck.erase(i);
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
			card->setScale(0.5);
			card ->setPosition(getDeckPosition());
			card->setState(STATE::DECK);
			addChild(card,-1);
			deck.pushBack(card);
		}
	}
	if (isJoker)
	{
		for (int i = 0; i < num; i++)
		{
			Card* joker = Card::create(MARK::JOKER_MARK,NUMBER::JOKER_NUMBER);
			joker->setScale(0.5);
			joker->setPosition(getDeckPosition());	
			joker->setState(STATE::DECK);
			addChild(joker,-1);
			deck.pushBack(joker);
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
		int iRand = random(0, (int)deck.size()-1);
		Card* card = Card::create(deck.at(iRand)->myMark,deck.at(iRand)->myNumber);
		card->setScale(0.5);
		card->setPosition(getDeckPosition());
		card->setState(STATE::DECK);
		deck.at(iRand)->removeFromParentAndCleanup(true);
		deck.erase(iRand);
		temp.pushBack(card);
		addChild(card, -1);
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

//ドロー
void Dealer::cardDrow(Vector<Card*>&hand)
{
	if (deck.size() <= 0) 
	{
		return; 
	}
	hand.pushBack(deck.at(0));
	deck.at(0)->setState(STATE::HAND);
	deck.erase(0);
	checkDeckZero();
};

//デッキから直接捨て札に送る
void Dealer::cardDeckThrow() 
{
	if (deck.size() > 0) 
	{
		grave.pushBack(deck.at(0));
		Card* card = Card::create(deck.at(0)->myMark, deck.at(0)->myNumber);
		card->setMyPosition(Vec2(designResolutionSize.width*0.6f+random(-30,30), designResolutionSize.height*0.5f+random(-30,30)));
		card->setScale(0.5f);
		addChild(card);
		deck.erase(0);
	}
};

//デッキに加える
void Dealer::cardAdd(MARK mark, NUMBER num)
{
	Card* card = Card::create(mark, num);
	card->setState(STATE::DECK);
	deck.pushBack(card);
	checkDeckZero();
};

//デッキから特定のものを取り除く
void Dealer::cardLose(MARK mark, NUMBER num)
{
	for (int i = 0; i < deck.size();i++)
	{
		if (deck.at(i)->myMark == mark&&deck.at(i)->myNumber == num)
		{
			deck.at(i)->setState(STATE::GRAVE);
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

};

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
