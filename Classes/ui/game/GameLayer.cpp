#include "GameLayer.h"
#include "cocos2d.h"
#include "../../resource/Resources.h"
#include "../../model/Player.h"
#include "../../controller/Controllers.h"
#include "../../model/puzzle/Element.h"
#include "../../model/CountDown.h"

USING_NS_CC;

GameLayer::GameLayer()
	:_eraseCountDown(nullptr)
	,_dropCountDown(nullptr)
	,_touchEnabled(false)
	,_dragSprite(nullptr)
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

	
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if(!getTouchEnabled())
		{
			return false;
		}
		Point p = touch->getLocation();
		
		int cellX = p.x / CELL_SIZE;
		int cellY = p.y / CELL_SIZE;

		Sprite *elementSprite = (Sprite *)getChildByTag(getElementSpriteTag(cellX, cellY));
		if(!elementSprite)
		{
			return false;
		}
		_dragSprite = elementSprite;
		_dragSprite->setLocalZOrder(_dragSprite->getLocalZOrder() + 1);

		return true;
	};

	listener->onTouchMoved = [=](Touch *touch, Event *event)
	{
		Point p = touch->getLocation();
		if(_dragSprite){
			if(p.x < CELL_SIZE / 2)
			{
				p.x = CELL_SIZE / 2;
			}
			if(p.y < CELL_SIZE / 2)
			{
				p.y = CELL_SIZE / 2;
			}
			if(p.x > Player::getCellMgr()->getSizeX() * CELL_SIZE - CELL_SIZE / 2){
				p.x = Player::getCellMgr()->getSizeX() * CELL_SIZE - CELL_SIZE/ 2;
			}
			if(p.y > Player::getCellMgr()->getSizeY() * CELL_SIZE - CELL_SIZE / 2){
				p.y = Player::getCellMgr()->getSizeY() * CELL_SIZE - CELL_SIZE / 2;
			}

			_dragSprite->setPosition(p);
		}
	};

	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		Point p = touch->getLocation();
		
		int toCellX = p.x / CELL_SIZE;
		int toCellY = p.y / CELL_SIZE;
		
		
		int tag = _dragSprite->getTag();
		int cellX = tag % Player::getCellMgr()->getSizeX();
		int cellY = tag / Player::getCellMgr()->getSizeX();

		if(Player::getCellMgr()->testSwap(cellX, cellY, toCellX, toCellY))
		{
			Player::getCellMgr()->swap(cellX, cellY, toCellX, toCellY);
		}
		else
		{
			int x = cellX * CELL_SIZE + CELL_SIZE / 2;
			int y = cellY * CELL_SIZE + CELL_SIZE / 2;

			_dragSprite->setPosition(x, y);
		}

		_dragSprite->setLocalZOrder(_dragSprite->getLocalZOrder() - 1);
	};

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

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
	Controllers::getGameController()->beginErase.connect(this, &GameLayer::onBeginErase);
	Controllers::getGameController()->erase.connect(this, &GameLayer::onErase);
	Controllers::getGameController()->beginDrop.connect(this, &GameLayer::onBeginDrop);
	Controllers::getGameController()->drop.connect(this, &GameLayer::onDrop);
	Controllers::getGameController()->swap.connect(this, &GameLayer::onSwap);
	Layer::onEnter();
}

void GameLayer::onExit()
{
	Layer::onExit();
	//取消事件观察者
	Controllers::getGameController()->start.disconnect(this);
	Controllers::getGameController()->start.disconnect(this);
	Controllers::getGameController()->beginErase.disconnect(this);
	Controllers::getGameController()->erase.disconnect(this);
	Controllers::getGameController()->beginDrop.disconnect(this);
	Controllers::getGameController()->drop.disconnect(this);
	Controllers::getGameController()->swap.disconnect(this);
}

void GameLayer::onStart()
{
	onBeginDrop();

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

				int elementSpriteTag = getElementSpriteTag(x, y);
				//removeChildByTag(elementSpriteTag);

				addChild(elementSprite, 30, elementSpriteTag);

				int pointY = y * CELL_SIZE + CELL_SIZE / 2;

				int time = Player::getCellMgr()->getSizeY() * TIME_PER_CELL;
				ActionInterval *move = MoveTo::create(time, Point(initX, pointY));

				elementSprite->runAction(Sequence::create(move, _dropCountDown->action(), NULL));
			}
		}
	}
}

int GameLayer::getElementSpriteTag(int x, int y){
	return x + y * Player::getCellMgr()->getSizeX();
}

void GameLayer::onBeginErase()
{
	CC_SAFE_RELEASE_NULL(_eraseCountDown);
	_eraseCountDown = CountDown::create(CC_CALLBACK_0(GameLayer::onAfterErase, this));
	_eraseCountDown->retain();
}

void GameLayer::onErase(int x, int y, bool eraseX, bool eraseY)
{
	Sprite *elementSprite = (Sprite *)getChildByTag(getElementSpriteTag(x, y));
	//播放消除动画
	Texture2D* explosion = Director::getInstance()->getTextureCache()->addImage("explosion.png");

	Vector<SpriteFrame*> spriteFrames;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(explosion, Rect(j * 64, i * 64, 64, 64));

			spriteFrames.pushBack(spriteFrame);
		}
	}
	Animation* animation = Animation::createWithSpriteFrames(spriteFrames, 0.05f);
	Animate * animate = Animate::create(animation);
	
	Sequence *seq = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, elementSprite)), _eraseCountDown->action(), NULL);

	elementSprite->runAction(seq);
}

void GameLayer::onAfterErase()
{
	CC_SAFE_RELEASE_NULL(_eraseCountDown);

	//下落元素
	Player::getCellMgr()->drop();
}

void GameLayer::onBeginDrop()
{
	setTouchEnabled(false);
	CC_SAFE_RELEASE_NULL(_dropCountDown);
	_dropCountDown = CountDown::create(CC_CALLBACK_0(GameLayer::onAfterDrop, this));
	_dropCountDown->retain();
}

void GameLayer::onDrop(int fromX, int fromY, int toX, int toY)
{
	Sprite *elementSprite = (Sprite *)getChildByTag(getElementSpriteTag(fromX, fromY));
	

	if(!elementSprite)
	{
		Element *element = Player::getCellMgr()->getElement(toX, toY);
		char spriteName[15];
		sprintf(spriteName, "Planet-%d.png", element->getTag());
				
		elementSprite = Sprite::create(spriteName);
		int initX = fromX * CELL_SIZE + CELL_SIZE / 2;
		int initY = fromY * CELL_SIZE + CELL_SIZE / 2;
		elementSprite->setPosition(Point(initX, initY));

		addChild(elementSprite, 30);
	}

	_moveTo(elementSprite, toX, toY);
}

void GameLayer::_moveTo(Sprite *elementSprite, int toX, int toY)
{
	int toPointX = toX * CELL_SIZE + CELL_SIZE / 2;
	int toPointY = toY * CELL_SIZE + CELL_SIZE / 2;

	int fromPointX = elementSprite->getPositionX();
	int fromPointY = elementSprite->getPositionY();

	float x_len = toPointX - fromPointX;
	float y_len = toPointY - fromPointY;

	float len = std::sqrt(x_len * x_len + y_len * y_len);

	float time = len / CELL_SIZE * TIME_PER_CELL;

	ActionInterval *move = MoveTo::create(time, Point(toPointX, toPointY));

	elementSprite->runAction(Sequence::create(move, _dropCountDown->action(), NULL));
	elementSprite->setTag(getElementSpriteTag(toX, toY));
}

void GameLayer::onAfterDrop()
{
	CC_SAFE_RELEASE_NULL(_dropCountDown);

	if(Player::getCellMgr()->mark())
	{
		Player::getCellMgr()->erase();
	}
	else
	{
		setTouchEnabled(true);
	}
}

void GameLayer::onSwap(int x1, int y1, int x2, int y2)
{
	onBeginDrop();

	Sprite *elementSprite1 = (Sprite *)getChildByTag(getElementSpriteTag(x1, y1));
	Sprite *elementSprite2 = (Sprite *)getChildByTag(getElementSpriteTag(x2, y2));

	_moveTo(elementSprite1, x2, y2);
	_moveTo(elementSprite2, x1, y1);
}