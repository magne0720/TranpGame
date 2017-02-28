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
	
}
