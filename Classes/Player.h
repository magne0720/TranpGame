#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Brain.h"
#include "EffectManager.h"

USING_NS_CC;

//プレイヤー
class Player :public Node 
{
private:

public:

	static Player* create();
	static Player* create(Player* &p);
	bool init();
	bool init(Player* &p);
	//カードの表示
	void cardDispHand(bool isReversed,int max=11);
	//カードの並び替え
	void cardSort(ROLE kind,Vector<Card*> &hand);
	//ドローする
	void cardDrow(Vector<Card*>&deck);
	//カードを捨てる
	void cardThrow(int num,Vector<Card*>&grave);
	//カードすべて捨てる
	void handDeath();
	//役の初期化
	void ressetRole();
	//役の調査
	void checkRole();
	//役の得点を計算する
	void calcRole(Vector<Card*> result);
	//最後に引いたカードの確認
	int checkLastCard();
	//元のデータとの競合
	void cardSortMarge(Vector<Card*> result);
	//役のできていない最後の役を調べる
	int checkNotRole();

	//得点
	int point;
	//選んだ番号
	int pickNumber;
	//選んだパイル
	STATE pickState;
	//並び替えの状態
	ROLE sortType;
	//ノックできるか
	bool brainEnd;
	//手札の位置
	Vec2 handPos[11];
	//エフェクト
	EffectManager* effect;
	//最後に引いたカード
	Card* lastCard;
	//手札の情報
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

