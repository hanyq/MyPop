#ifndef __POP_GAME_CONTROLLER_H__
#define __POP_GAME_CONTROLLER_H__

#include "../util/sigslot.h"

using namespace sigslot;

class GameController 
{
public:

	signal0<> start;

};

#endif