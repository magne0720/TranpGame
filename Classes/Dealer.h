#ifndef __DEALER_H__
#define __DEALER_H__

#include "cocos2d.h"
#include "Card.h"
#include "Player.h"

USING_NS_CC;

#define MARK_MAX 4
#define NUMBER_MAX 13

class Dealer :public Player
{
public :

	static Dealer* create(Vec2 deckPos,Vec2 gravePos);

	Sprite* deckSp,*graveSp;
	Vector<Card*> deck,grave;
	Vec2 deckPostion, gravePosition;

	bool init(Vec2 deckPos,Vec2 gravePos);

	//デッキのセット
	void setDeck(bool isJoker=false, int number=2);
	//デッキの位置設定
	void setDeckPosition(Vec2 deck,Vec2 grave);
	Vec2 getDeckPosition();
	Vec2 getGravePosition();
	//デッキのクリア
	void cardClear();
	//シャッフル
	void cardShuffle();
	//いくつかの山に分けてシャッフル
	void cardSplitShuffle(int num);
	//パーフェクトシャッフル
	void cardPerfectShuffle();
	//デッキから直接捨て札に送る
	void cardDeckThrow();
	//デッキに加える
	void cardAdd(Card* card);
	//デッキから特定のものを取り除く
	void cardLose(Card* card);
	//デッキの枚数がゼロかどうか
	void checkDeckZero();
	//捨て札の表示
	void cardDispGrave();
	//
	void ShuffleAnimation();



	bool onTouchBegan(const Touch * touch, Event *unused_event);
	void onTouchMoved(const Touch * touch, Event *unused_event);
	void onTouchEnded(const Touch * touch, Event *unused_event);

};


#endif // !__DEALER_H__
