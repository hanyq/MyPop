#ifndef __TRIPLE_PUZZLE_CELL__
#define __TRIPLE_PUZZLE_CELL__

#include<vector>
#include "cocos2d.h"
#include "model\CountDown.h"

USING_NS_CC;

#define time_per_cell 0.1
#define cell_size 64

class GameLayer;

class Cell
{
public:
	Cell();
	~Cell();

	inline int getTag() { return _tag; }

	inline void setTag(int tag) { _tag = tag; tagChanged(); }

	inline int getCountX() { return _countX; }

	inline void setCountX(int countX) { _countX = countX; }

	inline int getCountY(){ return _countY; }

	inline void setCountY(int countY){ _countY = countY; }

	inline Sprite* getSprite(){ return _sprite; }

	inline void setSprite(Sprite* sprite){ _sprite = sprite; }

	void tagChanged();

private:
	int _tag;
	int _countX;
	int _countY;
	Sprite* _sprite;
};

class TriplePuzzle{
public:

	TriplePuzzle(GameLayer* gameLayer, int x = 6, int y = 9, int types = 3);

	~TriplePuzzle();

	static TriplePuzzle* getInstance();

	//初始化三消格子
	void init();

	//添加新的Cell
	void addCells();

	//消除
	void eliminate();

	void swap(int x1, int y1, int x2, int y2, CountDown* countDown);

	void _moveTo(int oldX, int oldY, int x, int y, Cell* cell, CountDown* countDown);

	inline int getX(){ return _x; }

	inline int getY(){ return _y; }

	inline int getScore(){ return _score; }

	inline void setScore(int score){ _score = score; }

	void addScore(int score);

	inline int getTag(int x, int y)
	{
		Cell* cell = _cells[x][y];
		if (cell != nullptr)
			return cell->getTag();
		else
			return 0;
	}

	inline Cell* getCell(int x, int y)
	{
		return _cells[x][y];
	}

	inline void setCell(int x, int y, Cell* cell)
	{
		_cells[x][y] = cell;
	}

	void findCanMove();

	void markCanMove(int x1, int y1, int x2, int y2);

	void testAndMarkSwap(int x1, int y1, int x2, int y2);

	bool testSwap(int x1, int y1, int x2, int y2);

	bool testSwapFor(int x1, int y1, int x2, int y2);

private:
	int _x;
	int _y;
	int _types;
	Cell*** _cells;
	std::vector<Cell*> _eliminatedCells;
	GameLayer* _gameLayer;
	int _score;
	
	void _eliminateX(int x, int y, Cell* cell, CountDown* countDown);

	void _eliminateY(int x, int y, Cell* cell, CountDown* countDown);

	void _moveDownTo(int oldX, int oldY, int x, int y, Cell* cell, CountDown* countDown);

	void _eliminateCell(int x, int y, CountDown* countDown);
};

#endif