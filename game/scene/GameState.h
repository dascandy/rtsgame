#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ResourceManager.h"
#include "Texture.h"
#include "Coherent.h"
#include "EffectRenderPass.h"
#include "RenderTarget.h"

class Thread;

class GameState {
public:
	int seed;
	Res<Texture> texture;
	Res<Texture> texture2;
	Res<Texture> outflow;
	Res<Texture> outflow2;
	RenderTarget rt;
	RenderTarget rt2;
	EffectRenderPass trp;
	EffectRenderPass rain1, rain2;
	int initwaterlevel;
public:
	GameState(int seed);
	void update(int ms);
	float *terrainArr;
	vec2 xy, wh;
	int renderstep;
	enum RenderPhase {
		GenerateGround,
		SimulateRain,
		Done
	};
	RenderPhase phase;
private:
	void setupPhase(RenderPhase phase);
};

#endif

