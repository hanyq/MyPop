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

	//��Ϸ��ʼ
	void onStart();

	void cancelTouch();

	//�õ�λ�ã��룬 y����Ӧ��Ԫ�ؾ���Tag
	int getElementSpriteTag(int x, int y);

	//��ʼ����
	void onBeginErase();

	//����
	void onErase(int x, int y, bool eraseX, bool eraseY);

	//��������
	void onAfterErase();

	//��ʼ����
	void onBeginDrop();

	//����
	void onDrop(int fromX, int fromY, int toX, int toY);

	//�������
	void onAfterDrop();

private:
	CountDown *_eraseCountDown;//����
	CountDown *_dropCountDown;//����
};

#endif