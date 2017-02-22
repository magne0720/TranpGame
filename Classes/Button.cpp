#include "Button.h"



Button* Button::create(Vec2 buttonPos)
{
	Button *pRet = new Button();
	if (pRet && pRet->init(buttonPos))
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

bool Button::init(Vec2 buttonPos)
{
	if (!Sprite::init())
	{
		return false;
	}

	myRole = ROLE::ORDER;
	
	//�^�b�`����
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	initWithFile("mathsort.png");
	setMyPosition(buttonPos);
	return true;
}
void Button::setMyPosition(Vec2 pos)
{
	ButtonPosition = pos;
	MyPosition = pos;
	setPosition(pos);
};

void update(float delta)
{

}
bool Button::onTouchBegan(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		ModeChangeFor(myRole);
	}
	return true;
};

void Button::onTouchMoved(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{

	}
	else
	{

		ModeChange(myRole, false);
	}
	
};

void Button::onTouchEnded(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		ModeChange(myRole,true);
	}
	else
	{
		ModeChange(myRole, false);
	}
};

void Button::ModeChange(ROLE &kind,bool is)
{
	if (is)
	{
		if (kind == ROLE::ORDER) {
			setTexture("marksort.png");
			kind = ROLE::EQUAL;
		}
		else if (kind == ROLE::EQUAL) {
			setTexture("mathsort.png");
			kind = ROLE::ORDER;
		}
	}
	else
	{

		if (kind == ROLE::ORDER) {
			setTexture("mathsort.png");
		}
		else if (kind == ROLE::EQUAL) {
			setTexture("marksort.png");
		}
	}
	

};

void Button::ModeChangeFor(ROLE &kind)
{
	if (kind == ROLE::ORDER)
	{
		setTexture("mathfor.png");
	}
	else if (kind == ROLE::EQUAL)
	{
		setTexture("markfor.png");
	}
}