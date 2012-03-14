#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
	int seed;
public:
	GameState(int seed) : seed(seed), isReady(false) {}
	bool isReady;
};

#endif

