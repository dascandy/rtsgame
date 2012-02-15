#include "Game.h"

void Game::run()
{
	window.run();
}

Game::Game()
: window(1440, 900, false)
{
}

