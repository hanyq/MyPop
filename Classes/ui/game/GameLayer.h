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

	CC_SYNTHESIZE(bool, _touchEnabled, TouchEnabled);

	//��Ϸ��ʼ
	void onStart();

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

	//����λ��
	void onSwap(int x1, int y1, int x2, int y2);

private:

	void _moveTo(Sprite *elementSprite, int toX, int toY);
	CountDown *_eraseCountDown;//����
	CountDown *_dropCountDown;//����
	Sprite *_dragSprite;
};

#endif