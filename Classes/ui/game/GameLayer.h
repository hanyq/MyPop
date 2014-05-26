#ifndef __POP_GAME_LAYER_H__
#define __POP_GAME_LAYER_H__

#include "cocos2d.h"
#include "../../util/sigslot.h"

USING_NS_CC;
using namespace sigslot;

class GameLayer : public Layer, public has_slots<>
{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	static Scene * scene();

	virtual void onEnter();

	virtual void onExit();

	void onStart();

	void cancelTouch();

private:

};

#endif