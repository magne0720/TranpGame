#ifndef __MAINGAME_LAYER_H__
#define __MAINGAME_LAYER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Dealer.h"

USING_NS_CC;

//ジンラミーの基本手札枚数
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
	//ノックされたか
	bool isKnock;
	//ーーーーーーーーーーゲームの準備関数ーーーーーーーーー
	//誰が一番初めにゲームを始めるか
	void startPlayer();
	//カードを配る(初期カードの配置)
	void cardDivision();
	//スタート
	void gameStart();
	//ーーーーーーーーーーゲーム中に行う関数ーーーーーーーー
	//ターンチェンジ
	void NextPlayerTurn();
	//フェイズチェンジ
	void NextPhase();
	//ノック
	void callKnock();
	//ノック時に行われる役の計算
	void callCalculation();
	//ゲーム終了時の計算
	void callAddTotal();


	//ーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//タッチ判定
	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);


	Player* player_one, *player_two;
	Dealer* dealer;

};


#endif // !__MAINGAME_LAYER_H__
