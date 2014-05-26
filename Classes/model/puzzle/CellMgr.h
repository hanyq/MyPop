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

	//��ȡX�У�Y�еĸ���
	Cell * get(int x, int y);

	//��ȡ��X�У���Y�е�Ԫ��
	Element *getElement(int x, int y);

	void setElement(int x, int y, Element *element);

	//���¿�ʼ
	void start();

	int randTag();

private:

	void set(int x, int y, Cell* cell);

	Cell **_cells;//����
	Element **_elements;//Ԫ��
};


#endif