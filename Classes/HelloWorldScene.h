#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "Dealer.h"
#include "Player.h"
#include "SortButton.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);


	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	Player* p1, *p2;
	Dealer* dealer;
	SortButton* button;
	

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
