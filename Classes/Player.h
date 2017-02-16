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
	//�J�[�h���̂Ă�
	void cardThrow(Card* &card);
	//�J�[�h���ׂĎ̂Ă�
	void handDeath();
	//��������
	void checkFame(int x);
	//���q
	void checkOrder(int num);


	Vector<Card*> hand;
	int point;

};


#endif // !__PLAYER_H__

