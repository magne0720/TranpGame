#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "EffectManager.h"

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
	void cardDispHand(bool isReversed,int max=11);
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
	//�Ō�Ɉ������J�[�h�̊m�F
	int checkLastCard();
	//���U���g�ɓ���Ȃ��������̂�����
	void copyResultForHand(Player* &p);
	//�m�b�N���Ɏ̂Ă�J�[�h�̎�
	void setKnockThrowCard(Vector<Card*> c);

	//4�������ł����̂��ǂ���
	bool isFourCard;
	//���_
	int point;
	//�I�񂾔ԍ�
	int pickNumber;
	//�I�񂾃p�C��
	STATE pickState;
	//���ёւ��̏��
	ROLE sortType;
	//�m�b�N�ł��邩
	bool brainEnd;
	//�̂���������
	bool isKnocked;
	//�̂Ă�J�[�h�����܂��Ă��邩
	bool isDeside;
	//�J�E���g
	int brainCount;
	//���i���o�[�̃J�E���g
	int roleCount;
	//��D
	Vector<Card*>hand, result,chanceList;
	//��D�̈ʒu
	Vec2 handPos[11];
	//�G�t�F�N�g
	EffectManager* effect;
	//�Ō�Ɉ������J�[�h
	Card* lastCard;
	
	bool addPoint(int num);
	void setRoleColor(Vector<Card*> cResult);
	void checkRoleNew(Player* cHand);
	//�����o���Ȃ�
	void check(Player*& cHand, int x, int y);
	//�����ł���
	Player* check(Player*& cHand, int x, int y, int z);
	Player* check(Player*& cHand, int x, int y, int z,int q);
	void sort(Player* &p);
	void ressetPlayer();


};


#endif // !__PLAYER_H__

