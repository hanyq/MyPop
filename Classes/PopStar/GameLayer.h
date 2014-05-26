#ifndef __POP_STAR_GAME_LAYER__
#define __POP_STAR_GAME_LAYER__

#include "cocos2d.h"
#include "Puzzle.h"
#include "model/CountDown.h"

USING_NS_CC;

class GameLayer : public Layer
{
public:
	GameLayer();

	bool init();
	
	CREATE_FUNC(GameLayer);

	void enableTouch(bool touchEnabled);

	inline DrawNode* getDrawNode(){ return _drawNode; }

	void onScoreChanged(int score);

	~GameLayer();

private:
	Sprite* _sprite;
	Cell* _cell;
	int _beginX;
	int _beginY;
	int _lastX;
	int _lastY;
	EventListenerTouchOneByOne* _listener;
	DrawNode* _drawNode;
	int _moveSteps;
};

#endif