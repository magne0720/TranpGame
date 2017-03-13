#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;


//���X���������߂Ă���
//�ق����J�[�h���̂ĎD�ɂ��邩�ǂ������ׂāA����Ȃ���炤
//�K�v�̂Ȃ��J�[�h��K���Ɏ̂Ă�
//���x���ɂ���ăm�b�N���x��ς���
class Enemy:public Player
{
public :

	//���g���ق����J�[�h�Q
	Vector<Card*> wishCard;

	static  Enemy* create(int level);
	bool init(int level);
	
	//���x��
	int level;
	//�m�b�N����^�[����
	int knockTurn;
	
	//���̐ݒ�
	void setWishHand(int level);
	//�̂Ă�J�[�h�v�l
	void pickCardThink();
	//�����p�C���I��
	void setDrawState(STATE state);
	//�^�[���ɂ���Ĉ����p�C���I��
	STATE desidePile(Card* c,int turn,bool isPass=false);




};


#endif // !__ENEMY_H__
