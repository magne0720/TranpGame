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
	

	
	MainGameLayer* layer = MainGameLayer::create(level);
	addChild(layer);

	return true;
};

