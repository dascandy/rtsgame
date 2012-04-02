#include "GameState.h"
#include "Thread.h"
#include "Texture.h"
#include "Simplex.h"
#include <math.h>

static const int splits = 4;

GameState::GameState(int seed) 
: rt(2048, 2048, false)
, rt2(2048, 2048, false)
, texture(new Texture(2048, 2048, GL_RGBA32F))
, texture2(new Texture(2048, 2048, GL_RGBA32F))
, outflow(new Texture(2048, 2048, GL_RGBA32F))
, outflow2(new Texture(2048, 2048, GL_RGBA32F))
, trp(ResourceManager::Instance().getResource<ShaderProgram>("terrain"), rt)
, rain1(ResourceManager::Instance().getResource<ShaderProgram>("rain"), rt)
, rain2(ResourceManager::Instance().getResource<ShaderProgram>("rain"), rt2)
, renderstep(0)
, phase(GenerateGround)
, progress(new VarNum<float>(54))
{
	trp.Register("seed1", seed);
	trp.Register("seed1", seed);
	trp.Register("seed2", seed^0x5238765F);
	trp.Register("seed3", seed^0x7ba65093);
	trp.Register("seed4", seed^0x40a25918);
	trp.Register("seed5", seed^0x3929a20a);
	trp.Register("seed6", seed^0x297346C4);
	trp.Register("seed7", seed^0x12787a32);
	trp.Register("seed8", seed^0x29843E76);
	trp.Register("levels", 21);
	trp.Register("setwater", initwaterlevel);
	trp.Register("wh", wh);
	trp.Register("xy", xy);
	setupPhase(GenerateGround);
}

unsigned short GameState::lastObjId = 0;

unsigned short GameState::getObjectId() 
{
	return lastObjId++;
}

void GameState::setupPhase(RenderPhase newPhase) {
	switch(newPhase) {
		case GenerateGround:
			rt.AddTarget(texture);
			break;
		case SimulateRain:
			rt.AddTarget(outflow);
			rt2.AddTarget(texture2);
			rt2.AddTarget(outflow2);
			break;
	}
	phase = newPhase;
}

void GameState::update(int ms) {
	switch(phase) {
		case GenerateGround:
			if (trp.program.isLoaded()) {
				wh = vec2((1.0 / (1<<splits)), (1.0 / (1 << splits)));
				xy = vec2((1.0 / (1<<splits)) * (renderstep & ((1 << splits) - 1)), (1.0 / (1<<splits)) * (renderstep >> splits));
				trp.Run();
				renderstep++;
				delete progress;
				progress = new VarNum<float>(100.0f * renderstep / (1 << (2 * splits)));
				if (renderstep >= (1 << (2*splits)))
					setupPhase(SimulateRain);
			}
			break;
		case SimulateRain:
			setupPhase(Done);
			break;
			/*
			if (renderstep & 1) {
				rain1.Run();
			} else {
				rain2.Run();
			}
			renderstep++;
			if (renderstep >= rainphases) setupPhase(Done);
			break;
			*/
	}
}

