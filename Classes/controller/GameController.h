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

	//��Ϸ��ʼ
	signal0<> start;

	//��ʼ����Ԫ��
	signal0<> beginErase;

	//����Ԫ��
	signal4<int, int, bool, bool> erase;

	//��ʼ����Ԫ�أ���հ�
	signal0<> beginDrop;

	//����Ԫ��
	signal4<int, int, int, int> drop;

	//����Ԫ��
	signal4<int, int, int, int> swap;
};

#endif