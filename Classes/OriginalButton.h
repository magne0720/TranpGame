#ifndef __ORIGINAL_BUTTON_H__
#define __ORIGINAL_BUTTON_H__

#include "cocos2d.h"
#include "AllTags.h"

USING_NS_CC;

class OriginalButton :public Sprite
{
public:
	static OriginalButton* create(Vec2 passPos , int num);

	Sprite* pass,*nock,*pause;

	Vec2 MyPosition;
	Vec2 ButtonPosition;

	bool init(Vec2 passPos,int num);
	bool flg = false;

	//�{�^���`��
	void setDesign(int num);

	void setMyPosition(Vec2 passPos);

	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

	void ModeChange(ROLE &kind, bool is);

	void ModeChangeFor(ROLE &kind);
};
#endif
