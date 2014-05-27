#ifndef __POP_GAME_LAYER_H__
#define __POP_GAME_LAYER_H__

#include "cocos2d.h"
#include "../../util/sigslot.h"
#include "../../model/CountDown.h"

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

	//游戏开始
	void onStart();

	void cancelTouch();

	//得到位置（想， y）对应的元素精灵Tag
	int getElementSpriteTag(int x, int y);

	//开始消除
	void onBeginErase();

	//消除
	void onErase(int x, int y, bool eraseX, bool eraseY);

	//消除结束
	void onAfterErase();

	//开始下落
	void onBeginDrop();

	//下落
	void onDrop(int fromX, int fromY, int toX, int toY);

	//下落结束
	void onAfterDrop();

private:
	CountDown *_eraseCountDown;//消除
	CountDown *_dropCountDown;//下落
};

#endif