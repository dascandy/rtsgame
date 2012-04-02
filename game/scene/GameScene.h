#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "GameObject.h"
#include <vector>

class GameScene {
	std::vector<GameObject *> gos;
public:
	void Add(GameObject *go);
	void Gather(GameObject::gpu *gpustr, float *&bonelocs, float *&bonequats, int boneW);
};

#endif

