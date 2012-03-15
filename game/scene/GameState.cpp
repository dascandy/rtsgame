#include "GameState.h"
#include "Thread.h"
#include "Texture.h"
#include "Simplex.h"

GameState::GameState(int seed) 
: isReady(false) 
, rand(seed)
{
	createThread = new Thread();
	createThread->start(this, &GameState::create);
}

static double getTerrainVal(int i, int j) {
	float offset = 500 / 2048.0;
	double sum = 0;
	for (int k = 0; k < 11; k++) {
		sum += offset * simplex(i, j, 0);
	}
	return sum;
}

void GameState::create() {
	terrainArr = new float[2048*2048];

	for (int i = 0; i < 2048; i++) {
		for (int j = 0; j < 2048; j++) {
			terrain(i, j) = (float)getTerrainVal(i, j);
		}
	}

	isReady = true;
}

void GameState::createTexture() {
	texture = new Texture(2048, 2048, GL_RG32F);
	texture->SetContent(terrainArr);
}

