#include "Game.h"
#include "GameView.h"
#include "GameState.h"
#include "ObjectTypeDatabase.h"
#include "ObjectType.h"
#include "Object.h"
#include "Ground.h"

void Game::run()
{
	window.Run();
}

Game::Game()
: window(1600, 900, false)
{
	ObjectType &applecrate = ObjectTypeDatabase::Instance()["applecrate"];
	ObjectType &fern = ObjectTypeDatabase::Instance()["fernv2"];
	ObjectType &car = ObjectTypeDatabase::Instance()["truck"];
	ObjectType &barrel = ObjectTypeDatabase::Instance()["metal_barrel_blue"];
	ObjectType &woodbarrel = ObjectTypeDatabase::Instance()["woodbarrel_large"];
	ObjectType &tirestack = ObjectTypeDatabase::Instance()["tirestack"];
	ObjectType &palmplant = ObjectTypeDatabase::Instance()["palmplant"];
	ObjectType &tropicalplant = ObjectTypeDatabase::Instance()["tropicalplant"];
	ObjectType &gastank = ObjectTypeDatabase::Instance()["gastank"];
	std::vector<Vector3> pos;
	std::vector<Quaternion> quats;
	pos.push_back(Vector3(0,1,0));
	quats.push_back(Quaternion(0, 0, 0));
	pos[0].x = 0;
	pos[0].z = 0;
	pos[0].y = 20;
	Scene::Instance().AddObject(car.Create(pos, quats));
	pos[0].y = 1;

	pos[0].x = -3;
	pos[0].z = -3;
	Scene::Instance().AddObject(applecrate.Create(pos, quats));
	pos[0].x = -1;
	pos[0].z = -3;
	Scene::Instance().AddObject(fern.Create(pos, quats));
	pos[0].x = 1;
	pos[0].z = -3;
	Scene::Instance().AddObject(barrel.Create(pos, quats));
	pos[0].x = 3;
	pos[0].z = -3;
	Scene::Instance().AddObject(woodbarrel.Create(pos, quats));
	pos[0].x = -3;
	pos[0].z = 3;
	Scene::Instance().AddObject(tirestack.Create(pos, quats));
	pos[0].x = -1;
	pos[0].z = 3;
	Scene::Instance().AddObject(palmplant.Create(pos, quats));
	pos[0].x = 1;
	pos[0].z = 3;
	Scene::Instance().AddObject(tropicalplant.Create(pos, quats));
	pos[0].x = 3;
	pos[0].z = 3;
	Scene::Instance().AddObject(gastank.Create(pos, quats));
	Scene::Instance().SetGround(new Ground());
}

