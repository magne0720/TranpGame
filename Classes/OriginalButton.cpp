#include "OriginalButton.h"



OriginalButton* OriginalButton::create(Vec2 pos)
{
	OriginalButton *pRet = new OriginalButton();
	if (pRet && pRet->init(pos))
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

bool OriginalButton::init(Vec2 buttonpos)
{
	if (!Sprite::init())
	{
		return false;
	}
	//ƒ^ƒbƒ`”»’è
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(OriginalButton::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OriginalButton::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OriginalButton::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	initWithFile("pass.png");
	setMyPosition(buttonpos);

};

void OriginalButton::setMyPosition(Vec2 buttonPos)
{
	ButtonPosition = buttonPos;
	MyPosition = buttonPos;
	setPosition(buttonPos);
};

bool OriginalButton::onTouchBegan(const Touch * touch, Event *unused_event)
{
	return true;
};
void OriginalButton::onTouchMoved(const Touch * touch, Event *unused_event)
{

};
void OriginalButton::onTouchEnded(const Touch * touch, Event *unused_event)
{

}
