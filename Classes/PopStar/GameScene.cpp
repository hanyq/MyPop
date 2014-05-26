#include "GameScene.h"
#include "Puzzle.h"
#include "GameLayer.h"

USING_NS_CC;


bool GameScene::init(){
	if (!Scene::init()){
		return false;
	}


	Size visableSize = Director::getInstance()->getVisibleSize();
	auto backgroudLayer = LayerColor::create(ccColor4B(100, 67, 34, 100), 100, 100);

	addChild(backgroudLayer, 20);

	auto gameLayer = GameLayer::create(); //Layer::create();
	addChild(gameLayer, 10);
	
	//auto bg = Sprite::create("HelloWorld.png");
	//bg->setPosition(Point(visableSize.width / 2, visableSize.height / 2));

	//gameLayer->addChild(bg);

	auto label = LabelTTF::create("Hello World", "Arial", 24);
	gameLayer->addChild(label, 10);
	label->setPosition(Point(100, 100));

	TriplePuzzle* puzzle = new TriplePuzzle(gameLayer, 9, 15, 5);
	puzzle->init();
	puzzle->addCells();
	//puzzle->eliminate();
	//puzzle->addCells();
	
	//ccDrawLine(Point(0, 0), Point(100, 200));

	return true;
}

GameScene::~GameScene()
{
}
