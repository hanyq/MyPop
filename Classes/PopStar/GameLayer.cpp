#include "GameLayer.h"

#include "view\MenuLayer.h"

GameLayer::GameLayer()
:_moveSteps(0)
{
}

bool GameLayer::init()
{
	_drawNode = DrawNode::create();
	addChild(_drawNode, 500);

	auto label = LabelTTF::create("Score: 0 ", "Arial", 24);
	addChild(label, 1000, 100);
	Size size = Director::getInstance()->getWinSize();

	label->setPosition(Point(size.width / 2, size.height - 100));

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(false);
	listener->onTouchBegan = [=](Touch* pTouch, Event* pEvent)
	{
		_sprite = nullptr;
		int x = pTouch->getLocation().x;
		int y = pTouch->getLocation().y;

		int cellX = x / cell_size;
		int cellY = y / cell_size;

		if (cellX > TriplePuzzle::getInstance()->getX() - 1)
		{
			return false;
		}
		if (cellY > TriplePuzzle::getInstance()->getY() - 1)
		{
			return false;
		}
		_cell = TriplePuzzle::getInstance()->getCell(cellX, cellY);
		

		_sprite = _cell->getSprite();
		_sprite->setZOrder(_sprite->getZOrder() + 10);
		_beginX = cellX;
		_beginY = cellY;
		_lastX = cellX;
		_lastY = cellY;
		return true;
	};

	listener->onTouchMoved = [=](Touch* pTouch, Event* pEvent)
	{
		if (_sprite != nullptr)
		{
			float x = pTouch->getLocation().x;
			float y = pTouch->getLocation().y;

			if (x < cell_size / 2)
			{
				x = cell_size / 2;
			}
			if (x > TriplePuzzle::getInstance()->getX() * cell_size - cell_size / 2)
			{
				x = TriplePuzzle::getInstance()->getX() * cell_size - cell_size / 2;
			}

			if (y < cell_size / 2)
			{
				y = cell_size / 2;
			}
			if (y > TriplePuzzle::getInstance()->getY() * cell_size - cell_size / 2)
			{
				y = TriplePuzzle::getInstance()->getY() * cell_size - cell_size / 2;
			}


			_sprite->setPosition(Point(x, y));

			int cellX = x / cell_size;
			int cellY = y / cell_size;
			if (cellX == _lastX && cellY == _lastY)
			{
				return;
			}

			int currCellX = cell_size * (cellX + 0.5);
			int currCellY = cell_size * (cellY + 0.5);

			if (currCellX - x > cell_size * 0.25 ||
				currCellX - x < -cell_size * 0.25 ||
				currCellY - y > cell_size * 0.25 ||
				currCellY - y < cell_size * 0.25
				)
			{
				//return;
			}

			if (x > currCellX + cell_size * 0.35
				|| x < currCellX - cell_size * 0.35
				|| y > currCellY + cell_size* 0.35
				|| y < currCellY - cell_size * 0.35)
			{
				return;
			}

			if (cellX - _lastX > 1 
				|| cellX - _lastX < -1 
				|| cellY - _lastY > 1 
				|| cellY - _lastY < -1)
			{
				return;
			}

			if (!TriplePuzzle::getInstance()->testSwap(cellX, cellY, _lastX, _lastY))
			{
				return;
			}

			_moveSteps++;

			CountDown* countDown = CountDown::create(CC_CALLBACK_0(TriplePuzzle::eliminate, TriplePuzzle::getInstance()));
			Cell* cell = TriplePuzzle::getInstance()->getCell(cellX, cellY);
			TriplePuzzle::getInstance()->setCell(cellX, cellY, nullptr);
			TriplePuzzle::getInstance()->setCell(_lastX, _lastY, nullptr);
			TriplePuzzle::getInstance()->_moveTo(cellX, cellY, _lastX, _lastY, cell, countDown);
			TriplePuzzle::getInstance()->_moveTo(_lastX, _lastY, cellX, cellY, _cell, countDown);
			
			this->enableTouch(false);
			this->stopAllActions();
			_lastX = cellX;
			_lastY = cellY;
		}

	};

	listener->onTouchEnded = [=](Touch* pTouch, Event* pEvent)
	{
		_sprite->setPosition(Point((_lastX + 0.5) * cell_size, (_lastY + 0.5) * cell_size));
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto keybordListener = EventListenerKeyboard::create();
	keybordListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		UserDefault::getInstance()->flush();
		Director::getInstance()->end();
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybordListener, this);

	_listener = listener;
	listener->retain();

	return true;
}


void GameLayer::enableTouch(bool touchEnabled)
{
	if (touchEnabled)
	{
		if (_moveSteps < 15)
		{
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
		}
		else
		{
			auto menuLayer = MenuLayer::create();
			this->getParent()->addChild(menuLayer, 1000);
		}
	}
	else
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

void GameLayer::onScoreChanged(int score)
{
	int bestScore = UserDefault::getInstance()->getIntegerForKey("bestScore");

	if (_moveSteps == 10)
	{	
		if (bestScore < score)
		{
			bestScore = score;
			UserDefault::getInstance()->setIntegerForKey("bestScore", score);
			
		}
	}
	LabelTTF* label = (LabelTTF*)getChildByTag(100);
	char scores[50];
	sprintf(scores, "BestScore : %d, Score : %d, Step : %d", bestScore, score, _moveSteps);
	label->setString(scores);
}


GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE(_listener);
}
