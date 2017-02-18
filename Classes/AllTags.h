#ifndef __ALL_TAGS_H__
#define __ALL_TAGS_H__

#include "cocos2d.h"

#define DECK_TOP deck.size()-1
#define DECK_BOTTOM 0
#define GRAVE_TOP grave.size()-1
#define GRAVE_BOTTOM 0
#define HAND_SIZE hand.size()-1

//ゲームのターン制御
static enum TURN
{
	WAIT = 0, PLAY_ONE = 1, PLAY_TWO = 2, NUM
};
//ターンごとの制御
static enum PHASE
{
	START, DROW, THROW, KNOCK, END,
};
//カードの番号
static enum NUMBER
{
	ZERO = -1, ONE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, NUMBER_NUM, JOKER_NUMBER, ITEM_NUMBER
};
//カードのマーク
static enum MARK
{
	NONE = -1, SPADE = 1, CLOVER, HEART, DIAMOND, MARK_NUM, JOKER_MARK, ITEM_MARK
};
//カードの位置情報
static enum STATE
{
	DECK = 0, HAND, GRAVE
};

static enum ROLE
{
	WITHOUT = 0, ORDER, EQUAL
};

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);



#endif // !__ALL_TAGS_H__
