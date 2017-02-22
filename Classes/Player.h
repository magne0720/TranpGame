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
	//�J�[�h�̕\��
	void cardDispHand();
	//�J�[�h�̕��ёւ�
	void cardSort(ROLE kind);
	//�h���[����
	void cardDrow(Vector<Card*>&deck);
	//�J�[�h���̂Ă�
	void cardThrow(int num,Vector<Card*>&grave);
	//�J�[�h���ׂĎ̂Ă�
	void handDeath();
	//���̏�����
	void ressetRole();
	//���̒���
	int checkRole();
	//���̔���
	void chanceRole(Card* card,ROLE role,bool isAllCheck=false);
	//���̓��_���v�Z����
	int calcRole();
	//���q
	void checkOrder(int num);
	//���q
	void checkEqual(int num);
	//�O���̂Ƃ��납��J�[�h�𔲂��Ă������ǂ���
	void checkOrderExcept();
	//�O���̂Ƃ��납��J�[�h�𔲂��Ă������ǂ���
	void checkEqualExcept();
	//�l�����q�̈ꖇ����OK����
	void checkEqualFour(Card* card);
	//�l�����q�̈ꖇ����OK����
	void checkOrderFour(Card* card);

	Vector<Card*> hand,brain;
	//���̒��̌v�Z
	int brainHandCount;
	//���_
	int point;
	//�I�񂾔ԍ�
	int pickNumber;
	//�I�񂾃p�C��
	STATE pickState;
	//���ёւ��̏��
	ROLE sortType;

};


#endif // !__PLAYER_H__

