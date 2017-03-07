#include "EffectManager.h"

bool EffectManager::init() 
{
	if (!Node::init()) 
	{
		return false;
	}


	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-hand-out1.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-turn-over2.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-put1.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/knock.mp3");

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sound/card-shuffle2.mp3");


	for (int i = 0; i<=30; i++)
	{
		String* name = String::createWithFormat("Card/shuffle%02d.png", i%10);
		names.pushBack(name);
	};

	effectSp = Sprite::create();
	addChild(effectSp,1);

	limitNumber = 0;

	return true;
};

void EffectManager::update(float delta) 
{
	
};


EFFECT EffectManager::setCommonEffect(PHASE phase) 
{
	EFFECT e = EFFECT::DO_NOT;
	switch (phase)
	{
	case START:
		break;
	case DROW:
		e = EFFECT::DO_DRAW;
		break;
	case THROW:
		e = EFFECT::DO_THROW;
		break;
	case KNOCK:
		break;
	case END:
		break;
	case PASS:
		break;
	default:
		break;
	}
	return e;
};

void EffectManager::phaseChange(PHASE phase) 
{
	switch (phase)
	{
	case START:
		break;
	case DROW:
		SimpleAudioEngine::getInstance()->playEffect("Sound/card-turn-over2.mp3");
		break;
	case THROW:
		SimpleAudioEngine::getInstance()->playEffect("Sound/card-put1.mp3");
		break;
	case KNOCK:
		SimpleAudioEngine::getInstance()->playEffect("Sound/knock.mp3");
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
	card.at(limit)->setRotation(timer*720);
	if (timer >= 1)
	{
		card.at(limit)->setRotation(0);

		timer = 0;
		return true;
	}
	return false;
};

bool EffectManager::shuffleCard(Vec2 pos,float count,float limit, float delta)
{
	timer += delta;
	if (timer >= count)
	{
		if (limitNumber%10 == 1)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/card-shuffle2.mp3");
		}
		effectSp->setPosition(pos);
		effectSp->setTexture(names.at(limitNumber)->getCString());
		limitNumber++;
		timer = 0;
	}
	if (limitNumber > limit) 
	{
		timer = 0;
		limitNumber = 0;
		return true;
	}

	return false;
};
void EffectManager::drawLine(Sprite* card, bool up, bool down, bool left, bool right)
{
	Rect line = card->getBoundingBox();

	if (up) {
		DrawNode *enemyline = DrawNode::create();
		enemyline->drawSegment(Vec2(0, line.getMaxY()), Vec2(designResolutionSize.width, line.getMaxY()), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline, 0);
	}
	if (down) {
		DrawNode *enemyline2 = DrawNode::create();
		enemyline2->drawSegment(Vec2(0, line.getMinY()), Vec2(designResolutionSize.width, line.getMinY()), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline2, 0);
	}
	if (left) {
		DrawNode *enemyline3 = DrawNode::create();
		enemyline3->drawSegment(Vec2(line.getMinX(), 0), Vec2(line.getMinX(), designResolutionSize.height), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline3, 0);
	}
	if (right) {
		DrawNode *enemyline4 = DrawNode::create();
		enemyline4->drawSegment(Vec2(line.getMaxX(), 0), Vec2(line.getMaxX(), designResolutionSize.height), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline4, 0);
	}
}

void EffectManager::drawLine(Card* card, bool up, bool down, bool left, bool right)
{

	Rect line = card->getBoundingBox();
		

	//“G‘¤‚Ìü•`‰æ
	if (up) {
		DrawNode *enemyline = DrawNode::create();
		enemyline->drawSegment(Vec2(0, line.getMaxY()), Vec2(designResolutionSize.width, line.getMaxY()), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline, 0);
	}
	if (down) {
		DrawNode *enemyline2 = DrawNode::create();
		enemyline2->drawSegment(Vec2(0, line.getMinY()), Vec2(designResolutionSize.width, line.getMinY()), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline2, 0);
	}
	if (left) {
		DrawNode *enemyline3 = DrawNode::create();
		enemyline3->drawSegment(Vec2(line.getMinX(), 0), Vec2(line.getMinX(), designResolutionSize.height), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline3, 0);
	}
	if (right) {
		DrawNode *enemyline4 = DrawNode::create();
		enemyline4->drawSegment(Vec2(line.getMaxX(), 0), Vec2(line.getMaxX(), designResolutionSize.height), 1, Color4F(1, 1, 1, 1));
		addChild(enemyline4, 0);
	}

}

