#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ResourceManager.h"
#include "ObjectType.h"
#include "Color.h"

class Bone;
class GameScene;

class GameObject {
public:
	struct gpu {
		unsigned short objectId, modelId;
		unsigned short firstBoneX, firstBoneY;
		float or, og, ob;
		float tr, tg, tb;
	};
	GameObject(unsigned short objectId, ObjectType *type, float scale, Color color, Color teamcolor);
	~GameObject();
	void Write(gpu *str, float *&bonelocs, float *&bonequats, unsigned short &boneX, unsigned short &boneY, unsigned int boneW);
	unsigned short objectId;
	float scale;
	ObjectType *type;
	Color objColor;
	Color teamColor;
	Bone *bones;
};

#endif

