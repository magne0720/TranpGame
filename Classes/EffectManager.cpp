#include "EffectManager.h"

bool EffectManager::init() 
{
	if (!Node::init()) 
	{
		return false;
	}


	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-hand-out1.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-turn-over1.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-put1.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/knock.mp3");

	return true;
};

void EffectManager::update(float delta) 
{
	
};

void EffectManager::phaseChange(PHASE phase) 
{
	//SimpleAudioEngine::sharedEngine()->playEffect("Sound/card-hand-out1.mp3");
	switch (phase)
	{
	case START:
		break;
	case DROW:
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/card-turn-over1.mp3");
		break;
	case THROW:
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/card-put1.mp3");
		break;
	case KNOCK:
		SimpleAudioEngine::sharedEngine()->playEffect("Sound/knock.mp3");
		break;
	case END:
		break;
	case PASS:
		break;
	default:
		break;
	}

};

void EffectManager::openCard(int num)
{
	
};

bool EffectManager::drowCard(Vector<Card*>&card,int limit,Vec2 start,Vec2 end, float delta) 
{
	//üŒ`•âŠÔ
	timer += delta;
	card.at(limit)->setPosition((1 - timer)*start + timer*end);
	card.at(limit)->setRotation(timer*360);
	if (timer >= 1)
	{
		timer = 0;
		return true;
	}
	return false;
};

bool EffectManager::shuffleCard(Vec2 pos,float limit, float delta)
{
	timer += delta;
	if (timer>limit)return true;

	return false;
};
