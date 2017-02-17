#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

class Button :public Sprite
{
public:
	static Button* create(Vec2 buttonPos);

	Sprite* buttonSp;

	Vec2 ButtonPosition;
	bool init(Vec2 buttonPos);

	void setButtonPosition(Vec2 pos);
	Vec2 getButtonPosition();

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);



};

#endif
#pragma once
