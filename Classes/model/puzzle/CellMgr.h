#ifndef __POP_CELL_MGR_H__
#define __POP_CELL_MGR_H__

#include "cocos2d.h"
#include "../Mgr.h"
#include "Cell.h"
#include "Element.h"


USING_NS_CC;

class CellMgr : public Mgr
{
public:
	CellMgr();

	~CellMgr();

	CC_SYNTHESIZE(int, _sizeX, SizeX);

	CC_SYNTHESIZE(int, _sizeY, SizeY);

	CC_SYNTHESIZE(int, _tagCount, TagCount);

	void init(int sizeX, int sizeY);

	//获取X行，Y列的格子
	Cell * get(int x, int y);

	//获取第X行，第Y列的元素
	Element *getElement(int x, int y);

	void setElement(int x, int y, Element *element);

	//重新开始
	void start();

	int randTag();

private:

	void set(int x, int y, Cell* cell);

	Cell **_cells;//格子
	Element **_elements;//元素
};


#endif