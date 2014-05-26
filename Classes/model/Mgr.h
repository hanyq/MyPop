#ifndef __POP_MGR_H__
#define __POP_MGR_H__

#include "cocos2d.h"

USING_NS_CC;

class Mgr : public Ref
{
public:
	Mgr();

	virtual ~Mgr();

	virtual void restart();

private:

};

#endif __POP_MGR_H__