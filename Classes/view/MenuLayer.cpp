#include "MenuLayer.h"

#include "PopStar\GameScene.h"

MenuLayer::MenuLayer()
{
}


MenuLayer::~MenuLayer()
{
}

bool MenuLayer::init()
{
	MenuItem *restartItem = MenuItemFont::create("RESTART", CC_CALLBACK_0(MenuLayer::restart, this));
	Menu* mainMenu = Menu::create(restartItem, NULL);

	this->addChild(mainMenu);

	return true;
}

void MenuLayer::restart()
{
	auto gameScene = GameScene::create();

	Director::getInstance()->replaceScene(gameScene);
}