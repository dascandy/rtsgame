#include "Game.h"
#include "ResourceManager.h"
#include "GameState.h"

void Game::run()
{
	QueuedWork::Run(QueuedWork::OpenGL, 50);

	msec_t time = get_msec();

	for (;;) {
		msec_t curtime = get_msec();
		window.update(curtime - time);
		time = curtime;
		window.run();

		QueuedWork::Run(QueuedWork::OpenGL, 5);
		QueuedWork::Run(QueuedWork::Polling, -1);
	}
}

Game::Game()
: window(this, 1440, 900, false)
, gameState(0)
{
}

void Game::NewGame() {
	gameState = new GameState(6516518);
	window.gameView.setGameState(gameState);
	window.menuOpacity = fade(window.menuOpacity, new VarNum<float>(0), window.time, 1000, **window.time);
	window.inMenu = false;
}

void Game::SaveGame(const char *name) {
	TODO("Add saving game with name %s", name);
}

void Game::LoadGame(const char *name) {
	TODO("Add loading game with name %s", name);
}

