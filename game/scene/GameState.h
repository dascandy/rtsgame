#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Random.h"
#include "ResourceManager.h"
#include "Texture.h"

class Thread;

class GameState {
	int seed;
	Random rand;
	Thread *createThread;
	Res<Texture> texture;
public:
	GameState(int seed);
	void create();
	void createTexture();
	inline float &terrain(int x, int y) { return terrainArr[2048*y + x]; }
	bool isReady;
	float *terrainArr;
};

#endif

