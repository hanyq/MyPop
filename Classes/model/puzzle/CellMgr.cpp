#include "CellMgr.h"
#include <time.h>
#include "../../controller/Controllers.h"

CellMgr::CellMgr()
:_cells(nullptr)
, _elements(nullptr)
, _sizeX(0)
, _sizeY(0)
, _tagCount(0)
{
	srand((int)time(NULL));
}

CellMgr::~CellMgr()
{
	if (_cells)
	{
		for (int i = 0; i < _sizeX * _sizeY; i++)
		{
			CC_SAFE_RELEASE_NULL(_cells[i]);
		}
		CC_SAFE_DELETE(_cells);
	}
	if (_elements)
	{
		for (int i = 0; i < _sizeX * _sizeY; i++)
		{
			CC_SAFE_RELEASE_NULL(_elements[i]);
		}
		CC_SAFE_DELETE(_elements);
	}
}

void CellMgr::init(int sizeX, int sizeY)
{
	_sizeX = sizeX;
	_sizeY = sizeY;

	_tagCount = 5;//初始化5种类型的元素

	_cells = new Cell *[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		_cells[i] = Cell::create();
		_cells[i]->retain();
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			get(x, y)->initWith(x, y);
		}
	}

	_elements = new Element *[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		_elements[i] = nullptr;//设置每个格子的元素为NULL
	}
}

Cell *CellMgr::get(int x, int y)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	return _cells[x + y * _sizeX];
}

Element *CellMgr::getElement(int x, int y)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	return _elements[x + y * _sizeX];
}

void CellMgr::setElement(int x, int y, Element *element)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	CC_SAFE_RETAIN(element);
	CC_SAFE_RELEASE(_elements[x + y * _sizeX]);

	_elements[x + y * _sizeX] = element;
}

int CellMgr::randTag()
{
	return rand() % _tagCount + 1;
}

void CellMgr::start()
{
	for (int i = 0; i < _sizeX * _sizeY; i++)
	{
		_elements[i] = nullptr;//设置每个格子的元素为NULL
	}

	for (int x = 0; x < _sizeX; x++)
	{
		for (int y = 0; y < _sizeY; y++)
		{
			Element *element = Element::create();
			element->setX(x);
			element->setY(y);

			element->setTag(randTag());

			setElement(x, y, element);
		}
	}

	//抛出游戏开始事件
	Controllers::getGameController()->start();
}
