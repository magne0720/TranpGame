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
	void cardDispHand(bool isReversed);
	//�J�[�h�̕��ёւ�
	void cardSort(ROLE kind,Vector<Card*> &hand);
	//�h���[����
	void cardDrow(Vector<Card*>&deck);
	//�J�[�h���̂Ă�
	void cardThrow(int num,Vector<Card*>&grave);
	//�J�[�h���ׂĎ̂Ă�
	void handDeath();
	//���̏�����
	void ressetRole();
	//���̒���
	void checkRole();
	//���̓��_���v�Z����
	int calcRole(Vector<Card*> result);
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

	void addPoint(int num);


	Vector<Card*> hand,result;
	void checkRoleNew(Player* hand);
	Player* check(Player*& player, int x, int y, int z);
	Player* check(Player*& player, int x, int y, int z,int q);
	void sort(Vector<Card*> &card);
	bool brainEnd;
	int brainCount;

};


#endif // !__PLAYER_H__

