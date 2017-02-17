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
	if (!Node::init())
	{
		return false;
	}
	buttonSp = Sprite::create("button.png");
	buttonSp->setScale(1);
	setButtonPosition(buttonPos);
}

void Button::setButtonPosition(Vec2 pos)
{
	ButtonPosition = pos;
	buttonSp->setPosition(ButtonPosition);
};

Vec2 Button::getButtonPosition()
{
	return ButtonPosition;
}
void update(float delta)
{

}
bool Button::onTouchBegan(const Touch * touch, Event *unused_event)
{
	return true;
};

void Button::onTouchMoved(const Touch * touch, Event *unused_event)
{

};

void Button::onTouchEnded(const Touch * touch, Event *unused_event)
{

};