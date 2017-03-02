#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Brain.h"
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
	//���̃f�[�^�Ƃ̋���
	void cardSortMarge(Vector<Card*> result);
	//���̂ł��Ă��Ȃ��Ō�̖��𒲂ׂ�
	int checkNotRole();

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
	//��D�̈ʒu
	Vec2 handPos[11];
	//�G�t�F�N�g
	EffectManager* effect;
	//�Ō�Ɉ������J�[�h
	Card* lastCard;
	//��D�̏��
	Brain* myHand;

	bool addPoint(int num);
	void setRoleColor(Brain* cResult);
	void checkRoleNew(Brain* cHand);
	bool chance(Brain* cHand, int x, int y);
	Brain* check(Brain* cHand, int x, int y, int z);
	Brain* check(Brain* cHand, int x, int y, int z,int q);
	void sort(Brain* &p);
	void RessetPlayer();


};


#endif // !__PLAYER_H__

