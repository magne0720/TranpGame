#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "cocos2d.h"
#include "AllTags.h"

USING_NS_CC;

class EffectManager :public Node 
{
public:
	CREATE_FUNC(EffectManager);

	bool init();
	void update(float delta);

	void phaseChange(PHASE phase);
};


#endif //!__EFFECT_MANAGER_H__