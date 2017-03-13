#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "MainGameLayer.h"
#include "copyGameLayer.h"

USING_NS_CC;

class MainGameScene : public Scene
{
public:
	static Scene* createScene(int level);

	bool init(int level);



};

#endif // !__MAINGAME_SCENE_H__
