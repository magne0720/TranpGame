#ifndef __DEALER_H__
#define __DEALER_H__

#include "cocos2d.h"
#include "Card.h"
#include "Player.h"

USING_NS_CC;

#define MARK_MAX 4
#define NUMBER_MAX 13

class Dealer :public Player
{
public :

	static Dealer* create(Vec2 deckPos);

	Sprite* deckSp,graveSp;
	Vector<Card*> deck,grave;
	Vec2 deckPostion;

	bool init(Vec2 deckPos);

	//�f�b�L�̃Z�b�g
	void setDeck(bool isJoker=false, int number=2);
	//�f�b�L�̈ʒu�ݒ�
	void setDeckPosition(Vec2 pos);
	Vec2 getDeckPosition();
	//�f�b�L�̃N���A
	void cardClear();
	//�V���b�t��
	void cardShuffle();
	//�������̎R�ɕ����ăV���b�t��
	void cardSplitShuffle(int num);
	//�p�[�t�F�N�g�V���b�t��
	void cardPerfectShuffle();
	//�h���[������
	void cardDrow(Vector<Card*>&hand);
	//�f�b�L���璼�ڎ̂ĎD�ɑ���
	void cardDeckThrow();
	//�f�b�L�ɉ�����
	void cardAdd(MARK mark, NUMBER number);
	//�f�b�L�������̂��̂���菜��
	void cardLose(MARK mark,NUMBER num);
	//�f�b�L�̖������[�����ǂ���
	void checkDeckZero();
	//�̂ĎD�̕\��
	void cardDispGrave();



	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

};


#endif // !__DEALER_H__
