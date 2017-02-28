#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "AllTags.h"

USING_NS_CC;



class Card :public Sprite
{
public:
	
	static Card* create(MARK mark,NUMBER number);
	virtual bool init(MARK mark, NUMBER number);

	//自分の位置
	Vec2 myPosition;
	//自分の番号
	NUMBER myNumber;
	//自分のスート(4つのマーク)
	MARK myMark;
	//自分が今どこに所属しているか
	STATE myState;
	//自分がどの役か
	ROLE myRole;
	//役の番号
	int roleNumber;
	//表かどうか
	bool isReversed;
	//役として選ばれたか
	bool isFamed;
	//位置の設定
	void setMyPosition(Vec2 pos);
	Vec2 getMyPosition();
	//カードの設定
	void setKind(MARK mark,NUMBER number);
	void setDesign(int num,bool is=true);
	void setState(STATE state);
	void setRole(ROLE role);
	void setRoleNumber(int num);
	void setReverse(bool is);
	//裏返す
	void reverse();
};

#endif // !__CARD_H__
