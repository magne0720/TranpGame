#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
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
	//リザルトに入らなかったものを入れる
	void copyResultForHand(Player* &p);
	//ノック時に捨てるカード採取
	void setKnockThrowCard(Vector<Card*> c);

	//4枚役ができたのかどうか
	bool isFourCard;
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
	//のっくしたか
	bool isKnocked;
	//捨てるカードが決まっているか
	bool isDeside;
	//カウント
	int brainCount;
	//役ナンバーのカウント
	int roleCount;
	//手札
	Vector<Card*>hand, result,chanceList;
	//手札の位置
	Vec2 handPos[11];
	//エフェクト
	EffectManager* effect;
	//最後に引いたカード
	Card* lastCard;
	
	bool addPoint(int num);
	void setRoleColor(Vector<Card*> cResult);
	void checkRoleNew(Player* cHand);
	//役が出来ない
	void check(Player*& cHand, int x, int y);
	//役ができる
	Player* check(Player*& cHand, int x, int y, int z);
	Player* check(Player*& cHand, int x, int y, int z,int q);
	void sort(Player* &p);
	void ressetPlayer();


};


#endif // !__PLAYER_H__

