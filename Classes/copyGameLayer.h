#ifndef __COPYGAME_LAYER_H__
#define __COPYGAME_LAYER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Dealer.h"
#include "EffectManager.h"
#include "SortButton.h"
#include "OriginalButton.h"

USING_NS_CC;

//ジンラミーの基本手札枚数
#define HAND_START_MAX 10

class copyGameLayer :public Layer
{
public:
	TURN turn;
	PHASE phase;

	static copyGameLayer* create(int level);
	bool init(int level);
	void update(float delta);
	//ーーーーーーーーーーゲームの準備関数ーーーーーーーーー
	//誰が一番初めにゲームを始めるか
	void startPlayer();
	//カードを配る(初期カードの配置)
	void cardDivision();
	//スタート
	void gameStart();
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

	//タッチ判定
	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);


	Player* player_one, *player_two;
	Dealer* dealer;
	Label* phaseLabel, *turnLabel;
	EffectManager* effectManager;
	SortButton* button;
	OriginalButton* button2;//変更
	//ターンのカウント
	int turnCount;
	//ノックされたか(ゲームの終了条件)
	bool isKnock;
	//パスしたかどうか(先行のみ有効) 
	bool isPass;

};


#endif // !__copyGAME_LAYER_H__
