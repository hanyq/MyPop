#ifndef __POP_ELEMENT_H__
#define __POP_ELEMENT_H__

#include "cocos2d.h"

USING_NS_CC;

class Element : public Ref
{
public:
	Element();
	~Element();
	
	bool init();

	CREATE_FUNC(Element);

	CC_SYNTHESIZE(int, _type, Type);;

	CC_SYNTHESIZE(int, _tag, Tag);

	CC_SYNTHESIZE(int, _countX, CountX);

	CC_SYNTHESIZE(int, _countY, CountY);

	CC_SYNTHESIZE(int, _x, X);

	CC_SYNTHESIZE(int, _y, Y);

private:
	
};

#endif