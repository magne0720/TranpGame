#include "SortButton.h"



SortButton* SortButton::create(ROLE role,Vec2 buttonPos=Vec2(0,0))
{
	SortButton *pRet = new SortButton();
	if (pRet && pRet->init(role,buttonPos))
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

bool SortButton::init(ROLE role,Vec2 buttonPos)
{
	if (!Sprite::init())
	{
		return false;
	}
	//ƒ^ƒbƒ`”»’è
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SortButton::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SortButton::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SortButton::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	switchRole = role;

	
	initWithFile("Button/mathsort.png");

	setMyPosition(buttonPos);

	return true;
}

void SortButton::setMyPosition(Vec2 buttonPos)
{
	ButtonPosition = buttonPos;
	MyPosition = buttonPos;
	setPosition(buttonPos);
};

void update(float delta)
{

}
bool SortButton::onTouchBegan(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		ModeChangeFor(switchRole);
	}
	return true;
};

void SortButton::onTouchMoved(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{

	}
	else
	{

		ModeChange(switchRole, false);
	}
	
};

void SortButton::onTouchEnded(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		ModeChange(switchRole,true);
	}
	else
	{
		ModeChange(switchRole, false);
	}
};

void SortButton::ModeChange(ROLE &kind,bool is)
{
	if (is)
	{
		if (kind == ROLE::ORDER) 
		{
			setTexture("Button/mathsort.png");
			kind = ROLE::EQUAL;
		}
		else if (kind == ROLE::EQUAL)
		{
			setTexture("Button/marksort.png");
			kind = ROLE::ORDER;
		}
		
	}
	else
	{

		if (kind == ROLE::ORDER) {
			setTexture("Button/marksort.png");
		}
		else if (kind == ROLE::EQUAL) {
			setTexture("Button/mathsort.png");
		}
		
	}
	

};

void SortButton::ModeChangeFor(ROLE &kind)
{
	if (kind == ROLE::ORDER)
	{
		setTexture("Button/mathfor.png");
	}
	else if (kind == ROLE::EQUAL)
	{
		setTexture("Button/mathfor.png");
	}
};

