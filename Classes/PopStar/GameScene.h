#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"


class GameScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(GameScene);

	virtual bool init();


	~GameScene();
};

#endif