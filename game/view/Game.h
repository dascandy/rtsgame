#ifndef GAME_H
#define GAME_H

#include "GameWindow.h"

class Game
{
public:
	Game();
	void run();
	void SaveGame(const char *name);
	void LoadGame(const char *name);
	void NewGame();
private:
	GameWindow window;
	GameState *gameState;
};

#endif

