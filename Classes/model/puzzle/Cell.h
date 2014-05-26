#ifndef __POP_CELL_H__
#define __POP_CELL_H__

#include "cocos2d.h"

USING_NS_CC;

class Cell : public Ref
{
public:
	Cell();

	~Cell();

	virtual bool init();

	CREATE_FUNC(Cell);

	CC_SYNTHESIZE(int, _x, X);
	
	CC_SYNTHESIZE(int, _y, Y);

	CC_SYNTHESIZE(int, _countX, CountX);

	CC_SYNTHESIZE(int, _countY, CountY);

	void initWith(int x, int y);

private:

};

#endif