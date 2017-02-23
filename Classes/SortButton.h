#ifndef __SORT_BUTTON_H__
#define __SORT_BUTTON_H__

#include "cocos2d.h"
#include "Player.h"
#include "AllTags.h"

USING_NS_CC;

class SortButton :public Sprite
{
public:
	static SortButton* create(ROLE role,Vec2 buttonPos);

	ROLE switchRole;

	Vec2 ButtonPosition;
	Vec2 MyPosition;
	bool init(ROLE role,Vec2 buttonPos);
	bool isTouched;

	void setMyPosition(Vec2 buttonPos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	//�}�[�N���ԍ���
	void ModeChange(ROLE &kind,bool is);
	//�{�^�������Ă��
	void ModeChangeFor(ROLE &kind);
};

#endif
