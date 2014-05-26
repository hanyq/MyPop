#include "Cell.h"

Cell::Cell()
:_x(0)
, _y(0)
{

}

Cell::~Cell()
{

}


bool Cell::init()
{
	return true;
}

void Cell::initWith(int x, int y)
{
	_x = x;
	_y = y;
}