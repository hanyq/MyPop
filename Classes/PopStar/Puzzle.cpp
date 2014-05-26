#include "Puzzle.h"
#include <time.h>
#include "GameLayer.h"
#include "model/CountDown.h"

Cell::Cell()
:_tag(0)
, _countX(0)
, _countY(0)
, _sprite(nullptr)
{
}


Cell::~Cell()
{
	CC_SAFE_RELEASE_NULL(_sprite);
}

void Cell::tagChanged()
{
	char spriteName[15];
	if (_tag == 0){
		sprintf(spriteName, "Planet-%d.png", 1);
	}
	else
	{
		sprintf(spriteName, "Planet-%d.png", _tag);
	}

	if (_sprite == nullptr)
	{
		_sprite = Sprite::create(spriteName);
		CC_SAFE_RETAIN(_sprite);
	}
	else
	{
		_sprite->setTexture(TextureCache::getInstance()->addImage(spriteName));
	}
	
	//_sprite->setScale(Director::getInstance()-\)
	_sprite->setTextureRect(Rect(0, 0, 64, 64));
}


static TriplePuzzle* s_puzzle;

TriplePuzzle::TriplePuzzle(GameLayer* gameLayer, int x, int y, int types)
:_gameLayer(gameLayer)
,_x(x)
, _y(y)
, _types(types)
, _cells(nullptr)
, _eliminatedCells()
, _score(0)
{
	CC_SAFE_RETAIN(_gameLayer);
	_cells = new Cell**[x];
	for (int i = 0; i < x; i++)
	{
		_cells[i] = new Cell*[y];
		for (int j = 0; j < y; j++)
		{
			_cells[i][j] = nullptr;
		}
	}
	//srand(1);
	srand((int)time(NULL));

	s_puzzle = this;
}

TriplePuzzle* TriplePuzzle::getInstance()
{
	return s_puzzle;
}

TriplePuzzle::~TriplePuzzle()
{
	for (int i = 0; i < _x; i++)
	{
		for (int j = 0; j < _y; j++)
		{
			if (_cells[i][j] != nullptr)
			{
				delete _cells[i][j];
			}
		}
		delete[] _cells[i];
	}
	delete[] _cells;

	std::vector<Cell*>::iterator itr;
	for (itr = _eliminatedCells.begin(); itr != _eliminatedCells.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete *itr;
		}
	}

	CC_SAFE_RELEASE_NULL(_gameLayer);
}

void TriplePuzzle::init()
{
	//_cells = new int*
	for (int i = 0; i < _x * _y; i++)
	{
		Cell* cell = new Cell();
		cell->tagChanged();
		//cell->getSprite()->setVisible(false);
		_gameLayer->addChild(cell->getSprite(), 200);
		_eliminatedCells.push_back(cell);
	}
}

void TriplePuzzle::addCells()
{
	_gameLayer->enableTouch(false);

	CountDown* countDown = CountDown::create(CC_CALLBACK_0(TriplePuzzle::eliminate, this));
	int maxDistance = 0;
	for (int currX = 0; currX < _x; currX++)
	{
		for (int currY = 0; currY < _y; currY++)
		{
			if (_cells[currX][currY] == nullptr)
			{
				int avaliableY = currY + 1;
				for (; avaliableY < _y; avaliableY++)
				{
					if (_cells[currX][avaliableY] != nullptr)
					{
						Cell* avaliableCell = _cells[currX][avaliableY];
						_cells[currX][avaliableY] = nullptr;
						//向下移动
						_moveDownTo(currX, avaliableY, currX, currY++, avaliableCell, countDown);
					}
				}

				if (_y - currY > maxDistance)
				{
					maxDistance = _y - currY;
				}
				int i = 0;
				for (; currY < _y; currY++)
				{
					Cell* newCell = _eliminatedCells.back();
					_eliminatedCells.pop_back();

					int tag = rand() % _types + 1;
					newCell->setTag(tag);
					//添加新的Cell
					newCell->getSprite()->setVisible(true);
					_moveDownTo(currX, _y + i++, currX, currY, newCell, countDown);
				}
			}
		}
	}

	if (maxDistance == 0)
	{
		_gameLayer->enableTouch(true);
	}
}


void TriplePuzzle::_moveDownTo(int oldX, int oldY, int x, int y, Cell* cell, CountDown* countDown)
{
	_cells[x][y] = cell;
	Size spriteSize = cell->getSprite()->getContentSize();
	cell->getSprite()->setPosition(Point(cell_size * (0.5 + oldX), cell_size * (0.5 + oldY)));
	cell->getSprite()->setVisible(true);
	
	MoveTo* moveTo = MoveTo::create((oldY - y) * time_per_cell, Point(cell_size * (0.5 + x), cell_size * (0.5 + y)));
	countDown->incrCount();
	Sequence* seq = Sequence::create(moveTo, countDown, NULL);
	cell->getSprite()->runAction(seq);

	int realCountX = 1;
	int realCountY = 1;

	if (x > 0)
	{
		Cell *preCellX = _cells[x - 1][y];
		realCountX = (cell->getTag() == preCellX->getTag()) ? preCellX->getCountX() + 1 : 1;
	}
	if (y > 0)
	{
		Cell* preCellY = _cells[x][y - 1];
		realCountY = (cell->getTag() == preCellY->getTag()) ? preCellY->getCountY() + 1 : 1;
	}
	cell->setCountX(realCountX);
	cell->setCountY(realCountY);

	Cell* preCellX = cell;
	for (int currX = x + 1; currX < _x; currX++)
	{
		Cell* currCell = _cells[currX][y];
		if (currCell == nullptr){
			break;
		}

		int realCountX = (currCell->getTag() == preCellX->getTag()) ? preCellX->getCountX() + 1 : 1;

		//X正方向上，从currCell往后都不需要改变countX
		if (currCell->getCountX() == realCountX)
		{
			break;
		}

		currCell->setCountX(realCountX);

		preCellX = currCell;
	}

	
}


void TriplePuzzle::eliminate()
{
	_gameLayer->getDrawNode()->clear();
	_gameLayer->enableTouch(false);

	CountDown *countDown = CountDown::create(CC_CALLBACK_0(TriplePuzzle::addCells, this));
	bool hasEliminate = false;
	for (int currX = _x - 1; currX >= 0; currX--)
	{
		for (int currY = _y - 1; currY >= 0; currY--)
		{
			Cell* currCell = _cells[currX][currY];
			if (currCell == nullptr)
			{
				continue;
			}
			if (currCell->getCountX() >= 3 || currCell->getCountY() >= 3)
			{
				if (currCell->getCountX() >= 3)
				{
					_eliminateX(currX, currY, currCell, countDown);
					hasEliminate = true;
				}
				if (currCell->getCountY() >= 3)
				{
					hasEliminate = true;
					_eliminateY(currX, currY, currCell, countDown);
				}
				hasEliminate = true;
				_eliminateCell(currX, currY, countDown);
			}
		}
	}
	
	if (!hasEliminate)
	{
		_gameLayer->enableTouch(true);

		CallFunc* findCanMoveAction = CallFunc::create(CC_CALLBACK_0(TriplePuzzle::findCanMove, this));

		Sequence* seq = Sequence::create(DelayTime::create(5), findCanMoveAction, NULL);
		_gameLayer->runAction(seq);

		//findCanMove();
	}
}

void TriplePuzzle::findCanMove()
{
	for (int currX = _x - 1; currX >= 0; currX--)
	{
		for (int currY = _y - 1; currY >= 0; currY--)
		{
			testAndMarkSwap(currX, currY, currX - 1, currY);
			testAndMarkSwap(currX, currY, currX, currY - 1);
			testAndMarkSwap(currX, currY, currX - 1, currY - 1);
			if (currY + 1 < _y)
			{
				testAndMarkSwap(currX, currY, currX - 1, currY + 1);
			}
		}
	}
}

void TriplePuzzle::testAndMarkSwap(int x1, int y1, int x2, int y2)
{
	if (testSwap(x1, y1, x2, y2))
	{
		markCanMove(x1, y1, x2, y2);
	}
}

bool TriplePuzzle::testSwap(int x1, int y1, int x2, int y2)
{
	if (x2 < 0 || y2 < 0)
	{
		return false;
	}
	Cell* cell1 = _cells[x1][y1];
	Cell* cell2 = _cells[x2][y2];
	if (cell1->getTag() == cell2->getTag())
	{
		return false;
	}

	return testSwapFor(x1, y1, x2, y2) || testSwapFor(x2, y2, x1, y1);
}

bool TriplePuzzle::testSwapFor(int x1, int y1, int x2, int y2)
{
	
	Cell* cell1 = _cells[x1][y1];
	Cell* cell2 = _cells[x2][y2];

	//cell2放到Cell1位置 测试是否有可行
	int tag2 = cell2->getTag();
	int tempCountX = 1;
	int tempCountY = 1;
	if (x1 > 0)
	{
		Cell* left = _cells[x1 - 1][y1];
		if (left != cell2)
		{
			if (left->getTag() == tag2)
			{
				tempCountX = left->getCountX() + 1;
				if (tempCountX >= 3)
				{
					return true;
				}
			}
		}
	}
	int currX = x1;
	while (currX < _x - 1)
	{
		Cell* right = _cells[++currX][y1];
		if (right == cell2)
		{
			break;
		}
		if (right->getTag() == tag2)
		{
			tempCountX += 1;
			if (tempCountX >= 3)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}

	if (y1 > 0)
	{
		Cell* buttom = _cells[x1][y1 - 1];
		if (buttom != cell2)
		{
			if (buttom->getTag() == tag2)
			{
				tempCountY = buttom->getCountY() + 1;
				if (tempCountY >= 3)
				{
					return true;
				}
			}
		}
	}
	int currY = y1;
	while (currY < _y - 1)
	{
		Cell* top = _cells[x1][++currY];
		if (top == cell2)
		{
			break;
		}
		if (top->getTag() == tag2)
		{
			tempCountY += 1;
			if (tempCountY >= 3)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}


void TriplePuzzle::markCanMove(int x1, int y1, int x2, int y2)
{
	//
	Point p1 = Point((x1 + 0.5) * cell_size, (y1 + 0.5) * cell_size);
	Point p2 = Point((x2 + 0.5) * cell_size, (y2 + 0.5) * cell_size);
	
	_gameLayer->getDrawNode()->drawSegment(p1, p2, 3, Color4F(1, 0.8, 0.4, 1));
}

void TriplePuzzle::_eliminateX(int x, int y, Cell* cell, CountDown* countDown)
{
	for (int currX = x - cell->getCountX() + 1; currX < x; currX++)
	{
		Cell* currCellX = _cells[currX][y];
		if (currCellX == nullptr)
		{
			continue;
		}
		if (currCellX->getCountY() >= 3)
		{
			_eliminateY(currX, y, currCellX, countDown);
		}

		_eliminateCell(currX, y, countDown);
	}
}

void TriplePuzzle::_eliminateY(int x, int y, Cell* cell, CountDown* countDown)
{
	for (int currY = y - cell->getCountY() + 1; currY < y; currY++)
	{
		Cell* currCellY = _cells[x][currY];
		if (currCellY == nullptr)
		{
			continue;
		}
		if (currCellY->getCountX() >= 3)
		{
			_eliminateX(x, currY, currCellY, countDown);
		}

		_eliminateCell(x, currY, countDown);
	}
}

//消除currCell
void TriplePuzzle::_eliminateCell(int x, int y, CountDown* countDown)
{
	Cell* currCell = _cells[x][y];
	int countX = currCell->getCountX();
	int countY = currCell->getCountY();
	if (countX == 3)
	{
		addScore(10 * 3);
	}
	else if (countX > 3)
	{
		addScore((countX - 3) * 20);
	}
	if (countY == 3)
	{
		addScore(30);
	}
	else if (countY > 3)
	{
		addScore((countY - 3) * 20);
	}


	_eliminatedCells.push_back(currCell);
	//auto blink = Blink::create(1, 3);
	//auto fadeOut = FadeOut::create(time_per_cell * 2);

	Texture2D* explosion = TextureCache::getInstance()->addImage("explosion.png");

	Vector<SpriteFrame*> spriteFrames;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(explosion, Rect(j * 64, i * 64, 64, 64));

			spriteFrames.pushBack(spriteFrame);
		}
	}
	Animation* animation = Animation::createWithSpriteFrames(spriteFrames, 0.05);
	Animate * animate = Animate::create(animation);



	//auto a = Sequence::create(fadeOut, fadeOut->reverse(), blink, NULL);
	countDown->incrCount();
	auto a = Sequence::create(animate, Hide::create(), countDown, NULL);
	currCell->getSprite()->runAction(a);
	//currCell->getSprite()->setVisible(false);
	_cells[x][y] = nullptr;
}


void TriplePuzzle::swap(int x1, int y1, int x2, int y2, CountDown* countDown)
{
	if (x1 == x2 && y1 == y2)
	{
		return;
	}
	Cell* cell1 = _cells[x1][y1];
	_cells[x1][y1] = nullptr;
	Cell* cell2 = _cells[x2][y2];
	_cells[x2][y2] = nullptr;

	_moveTo(x1, y1, x2, y2, cell1, countDown);
	_moveTo(x2, y2, x1, y1, cell2, countDown);
}


void TriplePuzzle::_moveTo(int oldX, int oldY, int x, int y, Cell* cell, CountDown* countDown)
{
	if (cell == nullptr)
	{
		return;
	}
	_cells[x][y] = cell;
	Size spriteSize = cell->getSprite()->getContentSize();
	cell->getSprite()->setPosition(Point(spriteSize.width * (0.5 + oldX), spriteSize.height * (0.5 + oldY)));
	cell->getSprite()->setVisible(true);
	int le = oldY - y;
	if (le < 0)
	{
		le = -le;
	}
	else if (le == 0)
	{
		le = oldX - x;
		if (le < 0)
		{
			le = -le;
		}
	}
	MoveTo* moveTo = MoveTo::create(le * time_per_cell, Point(spriteSize.width * (0.5 + x), spriteSize.height * (0.5 + y)));
	countDown->incrCount();
	Sequence* seq = Sequence::create(moveTo, countDown, NULL);
	cell->getSprite()->runAction(seq);

	int realCountX = 1;
	int realCountY = 1;

	if (x > 0)
	{
		Cell *preCellX = _cells[x - 1][y];
		if (preCellX != nullptr)
		{
		
		realCountX = (cell->getTag() == preCellX->getTag()) ? preCellX->getCountX() + 1 : 1;
		}
	}
	if (y > 0)
	{
		Cell* preCellY = _cells[x][y - 1];
		if (preCellY != nullptr)
		{
		
		realCountY = (cell->getTag() == preCellY->getTag()) ? preCellY->getCountY() + 1 : 1;
		}
	}
	cell->setCountX(realCountX);
	cell->setCountY(realCountY);

	Cell* preCellX = cell;
	for (int currX = x + 1; currX < _x; currX++)
	{
		Cell* currCell = _cells[currX][y];
		if (currCell == nullptr){
			break;
		}

		int realCountX = (currCell->getTag() == preCellX->getTag()) ? preCellX->getCountX() + 1 : 1;

		//X正方向上，从currCell往后都不需要改变countX
		if (currCell->getCountX() == realCountX)
		{
			break;
		}

		currCell->setCountX(realCountX);
		preCellX = currCell;
	}

	Cell* preCellY = cell;
	for (int currY = y + 1; currY < _y; currY++)
	{
		Cell* currCell = _cells[x][currY];
		if (currCell == nullptr){
			break;
		}

		int realCountY = (currCell->getTag() == preCellY->getTag()) ? preCellY->getCountY() + 1 : 1;

		//X正方向上，从currCell往后都不需要改变countX
		if (currCell->getCountY() == realCountY)
		{
			break;
		}

		currCell->setCountY(realCountY);
		preCellY = currCell;
	}

}

void TriplePuzzle::addScore(int score)
{
	_score += score;

	_gameLayer->onScoreChanged(_score);
}