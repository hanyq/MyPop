#include "GameLayer.h"
#include "cocos2d.h"
#include "../../resource/Resources.h"
#include "../../model/Player.h"
#include "../../controller/Controllers.h"
#include "../../model/puzzle/Element.h"
#include "../../model/CountDown.h"

USING_NS_CC;

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{

}

bool GameLayer::init()
{
	//注册事件观察者
	Controllers::getGameController()->start.connect(this, &GameLayer::onStart);

	Player::getCellMgr()->init(9, 12);
	Player::getCellMgr()->start();
	
	auto bgSprite = Sprite::create(GAME_BACKGROUND_IMG);
	bgSprite->setAnchorPoint(Point(0, 0));
	bgSprite->setAnchorPoint(Director::getInstance()->getVisibleOrigin());
	//bgSprite->setContentSize();
	Size winSize = Director::getInstance()->getWinSize();
	bgSprite->setScaleX(winSize.width / bgSprite->getContentSize().width);
	bgSprite->setScaleY(winSize.height / bgSprite->getContentSize().height);

	addChild(bgSprite, 0);

	return true;
}

Scene *GameLayer::scene()
{
	Scene *scene = Scene::create();

	GameLayer *gameLayer = GameLayer::create();

	scene->addChild(gameLayer, 20);

	return scene;
}

void GameLayer::onEnter()
{
	//注册事件观察者
	Controllers::getGameController()->start.disconnect(this);
	Controllers::getGameController()->start.connect(this, &GameLayer::onStart);
	Layer::onEnter();
}

void GameLayer::onExit()
{
	Layer::onExit();
	//取消事件观察者
	Controllers::getGameController()->start.disconnect(this);
}

void GameLayer::cancelTouch()
{
	Player::getCellMgr()->start();
	//设置可否触摸
}

void GameLayer::onStart()
{
	CountDown *countDown = CountDown::create(CC_CALLBACK_0(GameLayer::cancelTouch, this));

	for (int x = 0; x < Player::getCellMgr()->getSizeX(); x++)
	{
		float initX = x * CELL_SIZE + CELL_SIZE / 2;
		float initY = Player::getCellMgr()->getSizeY() * CELL_SIZE - CELL_SIZE / 2;
		for (int y = 0; y < Player::getCellMgr()->getSizeY(); y++)
		{
			Element *element = Player::getCellMgr()->getElement(x, y);

			if (element)
			{
				char spriteName[15];
				sprintf(spriteName, "Planet-%d.png", element->getTag());
				
				Sprite *elementSprite = Sprite::create(spriteName);
				elementSprite->setPosition(Point(initX, initY));

				int elementSpriteTag = x + y * Player::getCellMgr()->getSizeX();
				removeChildByTag(elementSpriteTag);

				addChild(elementSprite, 30, elementSpriteTag);

				int pointY = y * CELL_SIZE + CELL_SIZE / 2;

				int time = Player::getCellMgr()->getSizeY() * TIME_PER_CELL;
				ActionInterval *move = MoveTo::create(time, Point(initX, pointY));

				elementSprite->runAction(CCSequence::create(move, countDown->action(), NULL));
			}
		}
	}
}