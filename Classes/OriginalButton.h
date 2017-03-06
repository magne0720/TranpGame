#ifndef __ORIGINAL_BUTTON_H__
#define __ORIGINAL_BUTTON_H__

#include "cocos2d.h"
#include "AllTags.h"

USING_NS_CC;

class OriginalButton :public Sprite
{
public:
	static OriginalButton* create(Vec2 passPos , int num);

	Vec2 MyPosition;
	Vec2 ButtonPosition;
	int mode;

	bool init(Vec2 passPos,int num);
	bool isTouched = false;

	//ƒ{ƒ^ƒ“•`‰æ
	void setDesign(int num,bool is=false);

	void setMyPosition(Vec2 passPos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	void setMode(int num);

};
#endif
