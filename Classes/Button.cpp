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
	initWithFile("button.png");
	setButtonPosition(buttonPos);
}

	//ƒ^ƒbƒ`”»’è
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	initWithFile("button.png");
	setPosition(buttonPos);
	ButtonPosition = buttonPos;
	
	return true;
}
void Button::setMyPosition(Vec2 pos)
{
<<<<<<< HEAD
	ButtonPosition = pos;
=======
	MyPosition = pos;
>>>>>>> c324d71defda2c98d13567fdb7cec3603a78b28d
	setPosition(pos);
};

void update(float delta)
{

}
bool Button::onTouchBegan(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{

	}
	return true;
};

void Button::onTouchMoved(const Touch * touch, Event *unused_event)
{

};

void Button::onTouchEnded(const Touch * touch, Event *unused_event)
{

};