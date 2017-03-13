#ifndef __MAINGAME_LAYER_H__
#define __MAINGAME_LAYER_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Dealer.h"
#include "EffectManager.h"
#include "SortButton.h"
#include "OriginalButton.h"

USING_NS_CC;

//ジンラミーの基本手札枚数
#define HAND_START_MAX 10

class MainGameLayer:public Layer
{
public:
	TURN turn;
	PHASE phase;
	EFFECT commonEffect;

	static MainGameLayer* create(int level);
	bool init(int level);
	void update(float delta);
	//ーーーーーーーーーーゲームの準備関数ーーーーーーーーー
	//誰が一番初めにゲームを始めるか
	void startPlayer();
	//カードを配る(初期カードの配置)
	void cardDivision();
	//スタート
	void gameStart();
	//エンド
	void gameEnd();
	//ーーーーーーーーーーゲーム中に行う関数ーーーーーーーー
	//プレイヤーが行う行動
	bool actionPhase();	
	//ターンチェンジ
	void nextPlayerTurn();
	//フェイズチェンジ
	void nextPhase(bool isAction);
	//ノック
	void callKnock();
	//ノック時に行われる役の計算
	void callCalculation();
	//ゲーム終了時の計算
	void callAddTotal();


	//ーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//ーーーーーーーーー演出ーーーーーーーーーーーーーー

	//シャッフル
	bool cardShuffleDesign();
	//配る
	bool cardDivisionDesign();
	//最初の一枚を捨てる
	bool cardDivisionThrowDesign();
	//引く
	bool cardDrowDesign();
	//捨てる
	bool cardThrowDesign();

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//タッチ判定
	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);


	Player* player_one;
	Enemy *player_two;
	Dealer* dealer;
	Label* phaseLabel,*turnLabel, *P_ONE_LABEL, *P_TWO_LABEL;
	EffectManager* effectManager;
	SortButton* sortButton;
	OriginalButton *passButton,*knockButton,*pauseButton;
	Sprite* titleLogo, *backGround;
	//ターンのカウント
	int turnCount;
	//ノックされたか(ゲームの終了条件)
	bool isKnock;
	//パスしたかどうか(先行のみ有効) 
	bool isPass;
	//ゲームが始まったか
	bool isGameStart;
	//演出のための変数
	int one_hand, two_hand;
	//進行スピード
	float phaseTimer, phaseSpeed;

};


#endif // !__MAINGAME_LAYER_H__
