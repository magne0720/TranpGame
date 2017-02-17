#include "EffectManager.h"

bool EffectManager::init() 
{
	if (!Node::init()) 
	{
		return false;
	}




	return true;
};

void EffectManager::update(float delta) 
{

};

void EffectManager::phaseChange(PHASE phase) 
{
	log("phase=%d", phase);
};
