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

	//自分の位置
	Vec2 myPoisition;
	//自分の番号
	NUMBER myNumber;
	//自分のスート(4つのマーク)
	MARK myMark;
	//自分が今どこに所属しているか
	STATE myState;
	//自分がどの役か
	ROLE myRole;
	//表かどうか
	bool isReversed;
	//触られているかどうか
	bool isTouched;
	//役として選ばれたか
	bool isFamed;
	//位置の設定
	void setMyPosition(Vec2 pos);
	//カードの設定
	void setKind(MARK mark,NUMBER number);
	void setDesign(int num=0);
	void setState(STATE state);
	void setRole(ROLE role);
	//裏返す
	void reverse();

};

#endif // !__CARD_H__
