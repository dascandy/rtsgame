#include "GameObject.h"
#include "ObjectType.h"
#include "Bone.h"
#include "GameScene.h"

GameObject::GameObject(unsigned short objectId, ObjectType *type, float scale, Color color, Color teamcolor) 
: objectId(objectId)
, type(type)
, scale(scale)
, objColor(color)
, teamColor(teamcolor)
, bones(new Bone[type->templates.size()])
{
}

GameObject::~GameObject() {
	delete [] bones;
}

void GameObject::Write(gpu *str, float *&bonelocs, float *&bonequats, unsigned short &boneX, unsigned short &boneY, unsigned int boneW) {
	if (boneX + type->templates.size() > boneW) {
		bonelocs += 4 * (boneW - boneX);
		bonequats += 4 * (boneW - boneX);
		boneX = 0; boneY++;
	}
	str->firstBoneX = boneX;
	str->firstBoneY = boneY;
	boneX += (unsigned short)type->templates.size();

	str->objectId = objectId;
	str->modelId = type->gfxModel->id;
	str->or = objColor.r;
	str->og = objColor.g;
	str->ob = objColor.b;
	str->tr = teamColor.r;
	str->tg = teamColor.g;
	str->tb = teamColor.b;

	for (size_t i = 0; i < type->templates.size(); i++) {
		*bonelocs++ = bones[i].translation.x;
		*bonelocs++ = bones[i].translation.y;
		*bonelocs++ = bones[i].translation.z;
		*bonelocs++ = scale;
		*bonequats++ = bones[i].rotation.x;
		*bonequats++ = bones[i].rotation.y;
		*bonequats++ = bones[i].rotation.z;
		*bonequats++ = bones[i].rotation.w;
	}
}

