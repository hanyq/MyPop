#ifndef __COUNT_DOWN__
#define __COUNT_DOWN__

#include "cocos2d.h"

USING_NS_CC;

class CountDown : public ActionInstant
{
CC_CONSTRUCTOR_ACCESS:
	CountDown()
	:_count(0)
	,_function(nullptr)
	{
	}
	
	virtual ~CountDown();

public:
	bool init(const std::function<void()>& func);

	static CountDown* create(const std::function<void()>& func);

	inline int getCount(){ return _count; }

	inline void setCount(int count){ _count = count; }

	inline int incrCount(){ return ++_count; }

	virtual void descCount();
	
	ActionInstant *action();

	//
	// Overrides
	//
	virtual void update(float time) override;
	virtual CountDown* reverse() const override;
	virtual CountDown* clone() const override;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CountDown);

private:
	int _count;
	std::function<void()> _function;
};

#endif