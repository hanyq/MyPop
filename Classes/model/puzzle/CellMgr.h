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

	//�����ͬtag��Ԫ�أ������Ƿ��п���������Ԫ��
	bool mark();

	//����Ԫ��
	void erase();

	//����Ԫ�أ����ȱ
	void drop();

	//���λ�ã�x1, y1���ͣ�x2, y2���ܷ񽻻�Ԫ��
	bool testSwap(int x1, int y1, int x2, int y2);

	//������x1, y1���ͣ�x2, y2��λ��
	void swap(int x1, int y1, int x2, int y2);

private:
	int randTag();

	void set(int x, int y, Cell* cell);

	void _swap(int x1, int y1, int x2, int y2);

	//���X�᷽���Ͽ�������Ԫ��
	void markEraseX(int x, int y, int countX);
	
	//���Y�᷽���Ͽ�������Ԫ��
	void markEraseY(int x, int y, int countY);

	Cell **_cells;//����
	Element **_elements;//Ԫ��
};


#endif