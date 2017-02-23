#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	dealer = Dealer::create(Vec2(designResolutionSize.width*0.4f, designResolutionSize.height*0.5f),
		Vec2(designResolutionSize.width*0.6f, designResolutionSize.height*0.5f));
	addChild(dealer);

	p1 = Player::create();
	p1->setPosition(Vec2(designResolutionSize.width*0.2f, designResolutionSize.height*0.3f));
	addChild(p1);

	p2 = Player::create();
	p2->setPosition(Vec2(designResolutionSize.width*0.2f, designResolutionSize.height*0.7f));
	addChild(p2);

	button = OriginalButton::create(Vec2(designResolutionSize.width *0.1f, designResolutionSize.height*0.5f));
	addChild(button);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}



bool HelloWorld::onTouchBegan(const Touch * touch, Event *unused_event) 
{
	//if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
	/*	if (dealer->deck.size() <= 0)
		{
			p1->handDeath();
			p2->handDeath();
			dealer->setDeck(true);
		}
		else
		{
			dealer->cardDrow(p1->hand);
			dealer->cardDrow(p2->hand);
			p1->cardDispHand();
			p2->cardDispHand();
		}*/

	}



	return true;
};

void HelloWorld::onTouchMoved(const Touch * touch, Event *unused_event)
{

};

void HelloWorld::onTouchEnded(const Touch * touch, Event *unused_event)
{

};

