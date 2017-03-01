#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "cocos2d.h"
#include "AllTags.h"
#include "Card.h"

USING_NS_CC;

class EffectManager :public Node 
{
public:
	CREATE_FUNC(EffectManager);

	Vector<Card>* hand;
	float timer;
	int limitNumber;

	bool init();
	void update(float delta);

	void phaseChange(PHASE phase);
	//カードをめくるアニメーション
	void openCard(int num);
	//カードをドローするアニメーション
	bool drowCard(Vector<Card*>&card,int limit,Vec2 start,Vec2 end, float delta);
	//カードをシャッフルするアニメーション
	bool shuffleCard(Vec2 pos, float limit, float delta);

};


#endif //!__EFFECT_MANAGER_H__