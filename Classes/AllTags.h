#ifndef __ALL_TAGS_H__
#define __ALL_TAGS_H__

static enum TURN
{
	WAIT = 0, PLAY_ONE = 1, PLAY_TWO = 2, NUM
};
static enum PHASE
{
	START, DROW, THROW, KNOCK, END,
};

static enum NUMBER
{
	ZERO = -1, ONE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, NUMBER_NUM, JOKER_NUMBER, ITEM_NUMBER
};
static enum MARK
{
	NONE = -1, SPADE = 1, CLOVER, HEART, DIAMOND, MARK_NUM, JOKER_MARK, ITEM_MARK
};
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
