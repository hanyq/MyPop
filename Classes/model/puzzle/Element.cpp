#include "Element.h"


Element::Element()
:_type(0)
, _tag(0)
, _x(0)
, _y(0)
, _countX(0)
, _countY(0)
{
}

Element::~Element()
{
}

bool Element::init()
{

	return true;
}
