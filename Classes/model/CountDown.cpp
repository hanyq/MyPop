#include "CountDown.h"


CountDown::~CountDown()
{
}

CountDown* CountDown::create(const std::function<void()>& func)
{
	CountDown* ret = new CountDown();
	if (ret && ret->init(func))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}


bool CountDown::init(const std::function<void()>& func)
{
	_function = func;

	return true;
}

void CountDown::descCount(){
	CCASSERT(_count > 0, "CountDown.count <= 0");
	_count--;
	if (_count == 0)
	{
		if (_function)
		{
			_function();
		}
	}
}

ActionInstant *CountDown::action()
{
	incrCount();

	return this;
}

void CountDown::update(float time)
{
	descCount();
}

CountDown* CountDown::clone() const
{
	CountDown* r = new CountDown();
	if (_function)
	{
		r->init(_function);
	}

	r->autorelease();

	return r;
}


CountDown* CountDown::reverse() const
{
	return clone();
}