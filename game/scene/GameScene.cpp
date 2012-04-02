#include "GameScene.h"

void GameScene::Add(GameObject *go) {
	gos.push_back(go);
}

void GameScene::Gather(GameObject::gpu *gpustr, float *&bonelocs, float *&bonequats, int boneW) {
	unsigned short boneX = 0, boneY = 0;
	for (std::vector<GameObject *>::iterator it = gos.begin(); it != gos.end(); ++it) {
		(*it)->Write(gpustr, bonelocs, bonequats, boneX, boneY, boneW);
	}
}

