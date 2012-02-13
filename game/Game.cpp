#include "Game.h"
#include "GameView.h"
#include "GameState.h"
#include "ObjectTypeDatabase.h"
#include "ObjectType.h"
#include "Object.h"
#include "Ground.h"
#include "Scene.h"

void Game::run()
{
	window.run();
}

Game::Game()
: window(1440, 900, false)
{
	Scene::Instance().SetGround(new Ground());
}

