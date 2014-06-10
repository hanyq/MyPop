#ifndef __POP_PLAYER_H__
#define __POP_PLAYER_H__

#include "puzzle/CellMgr.h"
#include "user/UserMgr.h"

class CellMgr;
class UserMgr;

class Player
{
public:
	Player();
	~Player();

	static CellMgr *getCellMgr();

	static UserMgr *getUserMgr();

private:

};

#endif