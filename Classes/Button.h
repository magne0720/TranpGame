#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class Button :public Sprite
{
public:
	static Button* create(Vec2 buttonPos);

<<<<<<< HEAD
=======

>>>>>>> c324d71defda2c98d13567fdb7cec3603a78b28d
	Vec2 ButtonPosition;
	Vec2 MyPosition;
	bool init(Vec2 buttonPos);
	bool isTouched;

	void setMyPosition(Vec2 pos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);



};

#endif
