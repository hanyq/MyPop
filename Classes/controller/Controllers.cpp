#include "Controllers.h"

GameController gameController;

Controllers::Controllers()
{

}

Controllers::~Controllers()
{

}


GameController * Controllers::getGameController()
{
	return &gameController;
}