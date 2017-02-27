#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

//�v���C���[
class Player :public Node 
{
private:
public:
	static Player* create();
	static Player* create(Player* &p);
	bool init();
	bool init(Player* &p);
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
	void calcRole(Vector<Card*> result);
	//���̒��̌v�Z
	int brainCount;
	//���_
	int point;
	//�I�񂾔ԍ�
	int pickNumber;
	//�I�񂾃p�C��
	STATE pickState;
	//���ёւ��̏��
	ROLE sortType;
	//���̑g�ݕ���
	int RoleSplit;
	//��D�Ƃ���̖���񂪓��鉼�z��D
	Vector<Card*> hand,result;
	//�m�b�N�ł��邩
	bool brainEnd;


	bool addPoint(int num);
	void setRoleColor(Vector<Card*> cResult);
	void checkRoleNew(Player* player);
	Player* check(Player* &player, int x, int y, int z);
	Player* check(Player* &player, int x, int y, int z,int q);
	void sort(Vector<Card*> &cards);
	void RessetPlayer();


};


#endif // !__PLAYER_H__

