#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "cocos2d.h"
#include "AllTags.h"
#include "Card.h"
#include "Player.h"

USING_NS_CC;

class EffectManager :public Node 
{
public:
	CREATE_FUNC(EffectManager);

	Player* player;
	Vector<Card>* hand;

	bool init();
	void update(float delta);

	void phaseChange(PHASE phase);
	//カードをめくるアニメーション
	void openCard(int num);
};


#endif //!__EFFECT_MANAGER_H__