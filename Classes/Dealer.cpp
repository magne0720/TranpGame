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

	graveCount = 0;

	deckSp = Sprite::create("Card/reverse.png");
	deckSp->setScale(0.5f);
	addChild(deckSp,1);

	graveSp = Sprite::create("Card/grave.png");
	graveSp->setScale(0.5f);
	addChild(graveSp, 1);

	setDeckPosition(deckPos,gravePos);

	setDeck(true);

	//�^�b�`����
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Dealer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Dealer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Dealer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);



	return true;
};

//�f�b�L�̈ʒu�ݒ�
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
}

//�f�b�L�̃Z�b�g
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


//�V���b�t��
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

//�p�[�t�F�N�g�V���b�t��
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

//�f�b�L���璼�ڎ̂ĎD�ɑ���
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

//�f�b�L�ɉ�����
void Dealer::cardAdd(MARK mark, NUMBER num)
{
	Card* card = Card::create(mark, num);
	card->setState(STATE::DECK);
	deck.pushBack(card);
	checkDeckZero();
};

//�f�b�L�������̂��̂���菜��
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

//�f�b�L���[�������ǂ���
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
//�̂ĎD�̕\��
void Dealer::cardDispGrave() 
{
	if (grave.size() > 0) 
	{
		Card* card = Card::create(grave.at(graveCount)->myMark, grave.at(graveCount)->myNumber);
		addChild(card);
		if (graveCount < grave.size())
		{
			graveCount++;
		}
	}
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
