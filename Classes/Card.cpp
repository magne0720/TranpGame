#include "Card.h"

Card* Card::create(MARK mark,NUMBER num)
{
	Card *pRet = new Card();
	if (pRet && pRet->init(mark,num))
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

bool Card::init(MARK mark,NUMBER number) 
{
	if (!Sprite::init()) 
	{
		return false;
	}

	//ƒ^ƒbƒ`”»’è
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Card::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Card::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	initWithFile("Card/reverse.png");
	isReversed = true;
	isTouched = false;
	isFamed = false;
	setKind(mark, number);

	scheduleUpdate();

	return true;
};

void Card::update(float delta) 
{

};

void Card::setKind(MARK mark, NUMBER num)
{
	myMark = mark;
	myNumber = num;
	setDesign();
};

void Card::setDesign() 
{
	String* name = String::createWithFormat("");

	switch (myMark)
	{
	case SPADE:
		name = String::createWithFormat("Card/spade_%02d.png", (int)myNumber);
		break;
	case CLOVER:
		name = String::createWithFormat("Card/clover_%02d.png", (int)myNumber);
		break;
	case HEART:
		name = String::createWithFormat("Card/heart_%02d.png", (int)myNumber);
		break;
	case DIAMOND:
		name = String::createWithFormat("Card/diamond_%02d.png", (int)myNumber);
		break;
	case JOKER_MARK:
		name = String::createWithFormat("Card/joker.png", (int)myNumber);
		break;
	default:
		break;
	}
	if (isReversed) 
	{
		setTexture(name->getCString());
	}
	else
	{
		setTexture("Card/reverse.png");
	}
};

void Card::setState(STATE state) 
{
	myState = state;
};

void Card::reverse()
{
	if (myState != STATE::DECK)
	{
		if (isReversed)
		{
			isReversed = false;
			setDesign();
		}
		else
		{
			isReversed = true;
			setDesign();
		}
	}
};

void Card::setMyPosition(Vec2 pos) 
{
	myPoisition = pos;
	setPosition(pos);
};

bool Card::onTouchBegan(const Touch * touch, Event *unused_event) 
{
	if (myState == STATE::HAND) 
	{
		if (getBoundingBox().containsPoint(touch->getLocation()))
		{
			isTouched = true;
		}
	}
	return true;
};

void Card::onTouchMoved(const Touch * touch, Event *unused_event) 
{
	if (isTouched) 
	{
		setMyPosition(touch->getLocation());
	}
};

void Card::onTouchEnded(const Touch * touch, Event *unused_event) 
{
	isTouched = false;
};
