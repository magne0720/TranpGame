#include "OriginalButton.h"



OriginalButton* OriginalButton::create(Vec2 pos ,int num)
{
	OriginalButton *pRet = new OriginalButton();
	if (pRet && pRet->init(pos,num))
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

bool OriginalButton::init(Vec2 buttonSpPos,int num)
{
	if (!Sprite::init())
	{
		return false;
	}
	//タッチ判定
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(OriginalButton::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OriginalButton::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OriginalButton::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	setMode(num);
	setDesign(num);
	setMyPosition(buttonSpPos);
};

void OriginalButton::setMode(int num) 
{
	mode = num;
}

void OriginalButton::setDesign(int num, bool is)//trueで押された判定
{
	String* name = String::createWithFormat("");
	switch (num)
	{
	case 0://パス
		if(is)name->initWithFormat("Button/pass.png");
		else name->initWithFormat("Button/passfor.png");
		break;
	case 1://ノック
		if(is)name->initWithFormat("Button/knockfor.png");
		else name->initWithFormat("Button/knock.png");
		break;
	case 2://ポーズ
		if(is) name->initWithFormat("Button/pausefor.png");
		else name->initWithFormat("Button/pause.png");
		break;
	default:
		if (is) name->initWithFormat("Button/pass.png");
		else name->initWithFormat("Button/pass.png");
		break;
	}
	setTexture(name->getCString());
};

void OriginalButton::setMyPosition(Vec2 buttonSpPos)
{
	setPosition(buttonSpPos);
	MyPosition = buttonSpPos;
};

bool OriginalButton::onTouchBegan(const Touch * touch, Event *unused_event)
{
	//ボタンのあたり判定
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		setDesign(mode,true);
	}
	return true;
};
void OriginalButton::onTouchMoved(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation())){}
	else
	{
		setDesign(mode);
	}
};
void OriginalButton::onTouchEnded(const Touch * touch, Event *unused_event)
{
	if (getBoundingBox().containsPoint(touch->getLocation()))
	{
		setDesign(mode);
	}
}
