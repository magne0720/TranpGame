#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"

USING_NS_CC;

	static enum NUMBER
	{
		ZERO = -1, ONE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING ,NUMBER_NUM,JOKER_NUMBER
	};
	static enum MARK
	{
		NONE=-1,SPADE=1, CLOVER, HEART, DIAMOND,MARK_NUM,JOKER_MARK
	};
	static enum STATE
	{
		DECK=0, HAND, GRAVE
	};
	static enum ROLE 
	{
		WITHOUT=0,ORDER, EQUAL 
	};

	static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
	static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
	static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
	static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);


class Card :public Sprite
{
public:


	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);
	
	static Card* create(MARK mark,NUMBER number);
	virtual bool init(MARK mark, NUMBER number);
	void update(float delta);

	//�����̈ʒu
	Vec2 myPoisition;
	//�����̔ԍ�
	NUMBER myNumber;
	//�����̃X�[�g(4�̃}�[�N)
	MARK myMark;
	//���������ǂ��ɏ������Ă��邩
	STATE myState;
	//�������ǂ̖���
	ROLE myRole;
	//�\���ǂ���
	bool isReversed;
	//�G���Ă��邩�ǂ���
	bool isTouched;
	//���Ƃ��đI�΂ꂽ��
	bool isFamed;
	//�ʒu�̐ݒ�
	void setMyPosition(Vec2 pos);
	//�J�[�h�̐ݒ�
	void setKind(MARK mark,NUMBER number);
	void setDesign(int num=0);
	void setState(STATE state);
	void setRole(ROLE role);
	//���Ԃ�
	void reverse();

};

#endif // !__CARD_H__
