#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include <map>
#include <string>
#include <vector>
#include "ResourceManager.h"
#include "glm.h"
#include "Physics.h"
#include "GameModel.h"
#include "Color.h"

class BehaviourType;
class BoneTemplate;
class ConstraintTemplate;
class GameObject;
class GameState;

class ObjectType {
public:
	~ObjectType();
	GameObject *Create(GameState &scene, const std::vector<vec3> &translation, const std::vector<quat> &rotation, float scale, Color color, Color teamcolor);
	GameObject *Create(GameState &scene, const vec3 &translation, const quat &rotation, float scale, Color color, Color teamColor);
	void AddBoneTemplate(BoneTemplate *bt)
	{
		templates.push_back(bt);
	}
	void AddConstraintTemplate(ConstraintTemplate *ct)
	{
		constraints.push_back(ct);
	}

	Res<GameModel> gfxModel;
	std::vector<BoneTemplate *> templates;
	std::vector<ConstraintTemplate *> constraints;
private:
	std::string name;
};

#endif

