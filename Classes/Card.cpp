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
	initWithFile("Card/reverse.png");
	isFamed = false;
	setKind(mark, number);
	setRole(ROLE::WITHOUT);
	roleNumber = 100;
	return true;
};

void Card::setKind(MARK mark, NUMBER num)
{
	myMark = mark;
	myNumber = num;
	setDesign((int)myNumber);
};

void Card::setDesign(int num,bool is) 
{
	String* name = String::createWithFormat("Card/reverse.png");

	switch (myMark)
	{
	case SPADE:
		name = String::createWithFormat("Card/spade_num_%02d.png", num);
		break;
	case CLOVER:
		name = String::createWithFormat("Card/clover_num_%02d.png", num);
		break;
	case HEART:
		name = String::createWithFormat("Card/heart_num_%02d.png", num);
		break;
	case DIAMOND:
		name = String::createWithFormat("Card/diamond_num_%02d.png", num);
		break;
	case JOKER_MARK:
		name = String::createWithFormat("Card/joker.png", num);
		break;
	default:
		name = String::createWithFormat("Card/joker.png", num);
		break;
	}
	if (is) 
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
	switch (state)
	{
	case DECK:
		setZOrder(0);
		break;
	case HAND:
		setZOrder(2);
		break;
	case GRAVE:
		setZOrder(1);
		break;
	default:
		break;
	}
};

void Card::setRole(ROLE role) 
{
	myRole = role;
};

void Card::setRoleNumber(int num) 
{
	roleNumber = num;
}

void Card::setReverse(bool is) 
{
	setDesign((int)myNumber, is);
}

void Card::reverse()
{
	if (myState != STATE::DECK)
	{
		if (isReversed)
		{
			isReversed = false;
			setDesign((int)myNumber);
		}
		else
		{
			isReversed = true;
			setDesign((int)myNumber);
		}
	}
};

void Card::setMyPosition(Vec2 pos) 
{
	myPosition = pos;
	setPosition(pos);
};
