#include "Player.h"

CellMgr cellMgr;

Player::Player()
{

}

Player::~Player()
{

}

CellMgr *Player::getCellMgr()
{
	return &cellMgr;
}