#ifndef __POP_GAME_CONTROLLER_H__
#define __POP_GAME_CONTROLLER_H__

#include "../util/sigslot.h"
#include "../model/CountDown.h"
#include "cocos2d.h"

using namespace sigslot;

class GameController 
{
public:
	GameController();

	//游戏开始
	signal0<> start;

	//开始消除元素
	signal0<> beginErase;

	//消除元素
	signal4<int, int, bool, bool> erase;

	//开始下落元素，填补空白
	signal0<> beginDrop;

	//下落元素
	signal4<int, int, int, int> drop;

	//交换元素
	signal4<int, int, int, int> swap;
};

#endif