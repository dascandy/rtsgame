#include "Game.h"
#include "ResourceManager.h"

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

	for (;;) {
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
: window(1440, 900, false)
{
}

