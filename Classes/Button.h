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
	Sprite* sortButton,*passButton, *nockButton;

	ROLE myRole;
	Vec2 SortButtonPosition, PassButtonPosition, NockButtonPosition;
	Vec2 MyPosition;
	bool init(Vec2 buttonPos, Vec2 passbuttonPos, Vec2 nockbuttonPos);
	bool isTouched;

	void setDesign(int num);
	void setMyPosition(Vec2 buttonPos,Vec2 passbuttonPos, Vec2 nockbuttonPos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	//�}�[�N���ԍ���
	void ModeChange(ROLE &kind,bool is);
	//�{�^�������Ă��
	void ModeChangeFor(ROLE &kind);
};

#endif
