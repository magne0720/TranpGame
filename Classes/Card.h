#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "AllTags.h"

USING_NS_CC;



class Card :public Sprite
{
public:


	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);
	
	static Card* create(MARK mark,NUMBER number);
	virtual bool init(MARK mark, NUMBER number);

	//�����̈ʒu
	Vec2 myPosition;
	//�����̔ԍ�
	NUMBER myNumber;
	//�����̃X�[�g(4�̃}�[�N)
	MARK myMark;
	//���������ǂ��ɏ������Ă��邩
	STATE myState;
	//�������ǂ̖���
	ROLE myRole;
	//���̔ԍ�
	int roleNumber;
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
	void setDesign(int num,bool is=true);
	void setState(STATE state);
	void setRole(ROLE role);
	void setRoleNumber(int num);
	void setReverse(bool is);
	//���Ԃ�
	void reverse();

};

#endif // !__CARD_H__
