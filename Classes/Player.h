#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

//プレイヤー
class Player :public Node 
{
public:
	CREATE_FUNC(Player);
	bool init();
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
	int calcRole(Vector<Card*> result);
	//頭の中の計算
	int brainHandCount;
	//得点
	int point;
	//選んだ番号
	int pickNumber;
	//選んだパイル
	STATE pickState;
	//並び替えの状態
	ROLE sortType;

	void addPoint(int num);


	Vector<Card*> hand,result;
	void checkRoleNew(Player* hand);
	Player* check(Player*& player, int x, int y, int z);
	Player* check(Player*& player, int x, int y, int z,int q);
	void sort(Vector<Card*> &card);
	bool brainEnd;
	int brainCount;

};


#endif // !__PLAYER_H__

