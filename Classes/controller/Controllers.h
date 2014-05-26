#ifndef __POP_CONTROLLERS_H__
#define __POP_CONTROLLERS_H__

#include "GameController.h"

class Controllers
{
public:
	Controllers();
	~Controllers();

	static GameController *getGameController();

private:

};

#endif