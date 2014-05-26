#ifndef __POP_MENU_LAYER__
#define __POP_MENU_LAYER__

#include "cocos2d.h"

USING_NS_CC;

class MenuLayer : public Layer
{
public:
	MenuLayer();
	~MenuLayer();

	bool init();

	CREATE_FUNC(MenuLayer);

	void restart();
};

#endif