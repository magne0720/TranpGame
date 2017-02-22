#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
#include "Player.h"
#include "AllTags.h"

USING_NS_CC;

class Button :public Sprite
{
public:
	static Button* create(Vec2 buttonPos,Vec2 passbuttonPos,Vec2 nockbuttonPos);
	Sprite* sortButton, passButton;

	ROLE myRole;
	Vec2 ButtonPosition;
	Vec2 MyPosition;
	bool init(Vec2 buttonPos);
	bool isTouched;

	void setDesign(int num);
	void setMyPosition(Vec2 pos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	//マークか番号か
	void ModeChange(ROLE &kind,bool is);
	//ボタン押してる間
	void ModeChangeFor(ROLE &kind);
};

#endif
