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

	//标记相同tag的元素，返回是否有可以消除的元素
	bool mark();

	//消除元素
	void erase();

	//下落元素，填补空缺
	void drop();

	//检查位置（x1, y1）和（x2, y2）能否交换元素
	bool testSwap(int x1, int y1, int x2, int y2);

	//交换（x1, y1）和（x2, y2）位置
	void swap(int x1, int y1, int x2, int y2);

private:
	int randTag();

	void set(int x, int y, Cell* cell);

	void _swap(int x1, int y1, int x2, int y2);

	//标记X轴方向上可消除的元素
	void markEraseX(int x, int y, int countX);
	
	//标记Y轴方向上可消除的元素
	void markEraseY(int x, int y, int countY);

	Cell **_cells;//格子
	Element **_elements;//元素
};


#endif