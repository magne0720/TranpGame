#ifndef __MAINGAME_LAYER_H__
#define __MAINGAME_LAYER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Dealer.h"

USING_NS_CC;

//�W�����~�[�̊�{��D����
#define HAND_START_MAX 10

class MainGameLayer:public Layer
{
public:
	enum TURN
	{
		NONE=-1,WAIT=0,PLAY_ONE=1,PLAY_TWO=2,NUM
	};
	enum PHASE 
	{
		START, DROW, THROW, KNOCK, END, 
	};
	TURN turn;
	PHASE phase;

	static MainGameLayer* create(int level);
	bool init(int level);
	void update(float delta);
	//�m�b�N���ꂽ��
	bool isKnock;
	//�[�[�[�[�[�[�[�[�[�[�Q�[���̏����֐��[�[�[�[�[�[�[�[�[
	//�N����ԏ��߂ɃQ�[�����n�߂邩
	void startPlayer();
	//�J�[�h��z��(�����J�[�h�̔z�u)
	void cardDivision();
	//�X�^�[�g
	void gameStart();
	//�[�[�[�[�[�[�[�[�[�[�Q�[�����ɍs���֐��[�[�[�[�[�[�[�[
	//�^�[���`�F���W
	void NextPlayerTurn();
	//�t�F�C�Y�`�F���W
	void NextPhase();
	//�m�b�N
	void callKnock();
	//�m�b�N���ɍs������̌v�Z
	void callCalculation();
	//�Q�[���I�����̌v�Z
	void callAddTotal();


	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	//�^�b�`����
	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);


	Player* player_one, *player_two;
	Dealer* dealer;

};


#endif // !__MAINGAME_LAYER_H__
