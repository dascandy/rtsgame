#include "Game.h"
#include "ResourceManager.h"
#include "GameState.h"

void Game::run()
{
	for (; QueuedWork::Background.size();) {
		Queued *q = QueuedWork::Background.front();
		QueuedWork::Background.pop_front();
		q->run();
		delete q;
	}
	for (; QueuedWork::ResourceLoading.size();) {
		Queued *q = QueuedWork::ResourceLoading.front();
		QueuedWork::ResourceLoading.pop_front();
		q->run();
		delete q;
	}

	msec_t time = get_msec();

	for (;;) {
		msec_t curtime = get_msec();
		window.update(curtime - time);
		time = curtime;
		window.run();

		int i = 0;
		for (; i < 5 && QueuedWork::ResourceLoading.size(); i++) {
			Queued *q = QueuedWork::ResourceLoading.front();
			QueuedWork::ResourceLoading.pop_front();
			q->run();
			delete q;
		}
		for (; i < 8 && QueuedWork::Background.size(); i++) {
			Queued *q = QueuedWork::Background.front();
			QueuedWork::Background.pop_front();
			q->run();
			delete q;
		}

		for (std::vector<Queued *>::iterator it = QueuedWork::Polls.begin(); it != QueuedWork::Polls.end(); ++it) {
			(*it)->run();
		}
	}
}

Game::Game()
: window(this, 1440, 900, false)
, gameState(0)
{
}

void Game::NewGame() {
	gameState = new GameState(get_msec());
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

