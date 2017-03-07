#include "HelloWorldScene.h"

Scene* HelloWorldScene::createScene(int level)
{
	HelloWorldScene *pRet = new HelloWorldScene();
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

bool HelloWorldScene::init(int level)
{
	if (!Scene::init())
	{
		return false;
	}
	CopyGameLayer* layer = CopyGameLayer::create(0);
	addChild(layer);

	return true;
};
