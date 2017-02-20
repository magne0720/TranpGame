#ifndef __MAINGAME_LAYER_H__
#define __MAINGAME_LAYER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Dealer.h"
#include "EffectManager.h"

USING_NS_CC;

//�W�����~�[�̊�{��D����
#define HAND_START_MAX 10

class MainGameLayer:public Layer
{
public:
	TURN turn;
	PHASE phase;

	static MainGameLayer* create(int level);
	bool init(int level);
	void update(float delta);
	//�[�[�[�[�[�[�[�[�[�[�Q�[���̏����֐��[�[�[�[�[�[�[�[�[
	//�N����ԏ��߂ɃQ�[�����n�߂邩
	void startPlayer();
	//�J�[�h��z��(�����J�[�h�̔z�u)
	void cardDivision();
	//�X�^�[�g
	void gameStart();
	//�[�[�[�[�[�[�[�[�[�[�Q�[�����ɍs���֐��[�[�[�[�[�[�[�[
	//�v���C���[���s���s��
	bool actionPhase();	
	//�^�[���`�F���W
	void nextPlayerTurn();
	//�t�F�C�Y�`�F���W
	void nextPhase(bool isAction);
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
	Label* phaseLabel,*turnLabel;
	EffectManager* effectManager;
	//�^�[���̃J�E���g
	int turnCount;
	//�m�b�N���ꂽ��(�Q�[���̏I������)
	bool isKnock;
	//�p�X�������ǂ���(��s�̂ݗL��) 
	bool isPass;

};


#endif // !__MAINGAME_LAYER_H__
