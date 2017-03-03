#ifndef __ALL_TAGS_H__
#define __ALL_TAGS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;


//�Q�[���̃^�[������
enum TURN
{
	WAIT = 0, PLAY_ONE = 1, PLAY_TWO = 2, NUM
};
//�^�[�����Ƃ̃v���C���[�̍s���̐���
enum PHASE
{
	START, DROW, THROW, KNOCK, END,PASS
};
//�J�[�h�̔ԍ�
enum NUMBER
{
	NONUMBER=-1,ZERO = 0, ONE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, NUMBER_NUM, JOKER_NUMBER, ITEM_NUMBER
};
//�J�[�h�̃}�[�N
enum MARK
{
	NOMARK = -1, NONE = 0, SPADE = 1, CLOVER, HEART, DIAMOND, MARK_NUM, JOKER_MARK, ITEM_MARK
};
//�J�[�h�̈ʒu���ƃv���C���[�̑I����
enum STATE
{
	DECK = 0,GRAVE,HAND,FREE
};
//��
enum ROLE
{
	WITHOUT = 0, ORDER, EQUAL,ROLEIN,CHANCE
};
//�Q�[���i�s�ɂ����鉉�o����
enum EFFECT 
{
	DO_NOT=0, DO_SHUFFLE,DO_DIVISION,DO_DIVISION_THROW,DO_DRAW,DO_THROW,DO_SORT
};

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);



#endif // !__ALL_TAGS_H__
