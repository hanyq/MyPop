#include "Player.h"

CellMgr cellMgr;
UserMgr userMgr;

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

UserMgr *Player::getUserMgr()
{
	return &userMgr;
}