#include "MainGameScene.h"

Scene* MainGameScene::createScene(int level)
{
	MainGameScene *pRet = new MainGameScene();
	if (pRet && pRet->init(level))
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

bool MainGameScene::init(int level)
{
	if (!Scene::init())
	{
		return false;
	}
	

	//ƒ^ƒbƒ`”»’è
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MainGameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainGameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainGameScene::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);



	return true;
};



bool MainGameScene::onTouchBegan(const Touch * touch, Event *unused_event) 
{
	if (touch->getLocation().x > designResolutionSize.width*0.5f) 
	{
		MainGameLayer* layer = MainGameLayer::create(0);
		addChild(layer);
	}
	else
	{
		copyGameLayer* layer = copyGameLayer::create(0);
		addChild(layer);
	}
	return true;
};

void MainGameScene::onTouchMoved(const Touch * touch, Event *unused_event)
{

};

void MainGameScene::onTouchEnded(const Touch * touch, Event *unused_event)
{

};
