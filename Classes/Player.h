#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

//�v���C���[
class Player :public Node 
{
public:
	CREATE_FUNC(Player);
	bool init();
	void update(float delta);
	//�J�[�h�̕\��
	void cardDispHand();
	//�J�[�h�̕��ёւ�
	void cardSort(int num);
	//�h���[����
	void cardDrow(Vector<Card*>&deck);
	//�J�[�h���̂Ă�
	Card* cardThrow(int num,Vector<Card*>&grave);
	//�J�[�h���ׂĎ̂Ă�
	void handDeath();
	//���̒���
	void checkRole(int i=0);
	//���̔���
	void chanceRole(Card* card,ROLE role,bool isAllCheck=false);
	//���q
	void checkOrder(int num);


	Vector<Card*> hand,brain;
	int brainHandCount;
	int point;
	int pickNumber;

};


#endif // !__PLAYER_H__

