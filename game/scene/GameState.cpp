#include "GameState.h"
#include "ShaderProgram.h"
#include <SDL.h>
#include "Scene.h"
#include "Physics.h"
#include "Object.h"
#include "profile.h"

GameState::GameState() : ended(false), paused(true), gametime(0), relGameSpeed(1000), lastTicks(SDL_GetTicks())
{
}

void GameState::SetVars(ShaderProgram &sp)
{
	sp.Set("time", SDL_GetTicks());
	sp.Set("gametime", (unsigned int)(gametime / 1000));
}

void GameState::Update(bool active)
{
	Vector3 loc((float)int(gamePosition.x + 0.5), (float)int(gamePosition.y + 0.5), (float)int(gamePosition.z + 0.5));
	int deltaMs = (SDL_GetTicks() - lastTicks);

	if (active)
	{
		int deltaGameUs = deltaMs * relGameSpeed;
		unsigned int oldGameframeno = (unsigned int)((gametime / 50000) * 3 + 
									  ((gametime % 50000) / 16667));

		gametime += deltaGameUs;

		unsigned int newGameframeno = (unsigned int)((gametime / 50000) * 3 + 
									  ((gametime % 50000) / 16667));

		int deltaGameFrames = newGameframeno - oldGameframeno;
//		int subFrameTime = (int)(gametime - deltaGameFrames * 50000 / 3);

		for (int i = 0; i < deltaGameFrames; i++)
		{
			Scene::Instance().Update(gamePosition);
			IPhysics::Instance().PerformPhysics(0.016666666f);
		}
	}

	lastTicks += deltaMs;
}

