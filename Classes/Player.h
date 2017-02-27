#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Card.h"

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
	void cardDispHand(bool isReversed);
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
	//頭の中の計算
	int brainCount;
	//得点
	int point;
	//選んだ番号
	int pickNumber;
	//選んだパイル
	STATE pickState;
	//並び替えの状態
	ROLE sortType;
	//役の組み分け
	int RoleSplit;
	//手札とそれの役情報が入る仮想手札
	Vector<Card*> hand,result;
	//ノックできるか
	bool brainEnd;


	bool addPoint(int num);
	void setRoleColor(Vector<Card*> cResult);
	void checkRoleNew(Player* player);
	Player* check(Player* &player, int x, int y, int z);
	Player* check(Player* &player, int x, int y, int z,int q);
	void sort(Vector<Card*> &cards);
	void RessetPlayer();


};


#endif // !__PLAYER_H__

