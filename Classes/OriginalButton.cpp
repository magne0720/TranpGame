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

bool OriginalButton::init(Vec2 passPos,int num)
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

	for (num = 0; num < 3; num++)setDesign(num);
	setMyPosition(passPos);
};
void OriginalButton::setDesign(int num)
{
	switch (num)
	{
	case 0:
		pass = Sprite::create("Button/pass.png");
		pass->setPosition(Vec2(designResolutionSize.width * 0.95, designResolutionSize.height * 0.4f));
		addChild(pass,1);
		break;
	case 1:
		nock = Sprite::create("Button/nock.png");
		nock->setPosition(Vec2(designResolutionSize.width * 0.95, designResolutionSize.height * 0.6f));
		addChild(nock,0);
		break;
	case 2:
		pause = Sprite::create("Button/pause.png");
		pause->setPosition(Vec2(designResolutionSize.width * 0.95,designResolutionSize.height * 0.9f));
		addChild(pause,0);
		break;
	default:
		break;
	}
};

void OriginalButton::setMyPosition(Vec2 passPos)
{
	ButtonPosition = passPos;
	MyPosition = passPos;
	setPosition(passPos);
};

bool OriginalButton::onTouchBegan(const Touch * touch, Event *unused_event)
{
	//パスボタンのあたり判定
	if (pass->getBoundingBox().containsPoint(touch->getLocation()))
	{
		pass->setTexture("Button/passfor.png");
	}
	//ノックボタンのあたり判定
	if (nock->getBoundingBox().containsPoint(touch->getLocation()))
	{
		nock->setTexture("Button/nockfor.png");
	}
	//ポーズボタンのあたり判定
	if (pause->getBoundingBox().containsPoint(touch->getLocation()))
	{
		pause->setTexture("Button/pausefor.png");
	}

	return true;
};
void OriginalButton::onTouchMoved(const Touch * touch, Event *unused_event)
{
	if (pass->getBoundingBox().containsPoint(touch->getLocation())){}
	else
	{
		pass->setTexture("Button/pass.png");
	}
	if (nock->getBoundingBox().containsPoint(touch->getLocation())) {}
	else
	{
		nock->setTexture("Button/nock.png");
	}
	if (pause->getBoundingBox().containsPoint(touch->getLocation())) {}
	else
	{
		pause->setTexture("Button/pause.png");
	}
};
void OriginalButton::onTouchEnded(const Touch * touch, Event *unused_event)
{
	if (pass->getBoundingBox().containsPoint(touch->getLocation()))
	{
		pass->setTexture("Button/pass.png");
	}
	if (nock->getBoundingBox().containsPoint(touch->getLocation()))
	{
		nock->setTexture("Button/nock.png");
	}
	if (pause->getBoundingBox().containsPoint(touch->getLocation()))
	{
		pause->setTexture("Button/pause.png");
	}

}
