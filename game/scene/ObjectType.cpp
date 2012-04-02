#include "ObjectType.h"
#include "Object.h"
#include "GameObject.h"
#include "Bone.h"
#include "BoneTemplate.h"
#include "GameState.h"
#include "ConstraintTemplate.h"

ObjectType::~ObjectType()
{
	for (std::vector<BoneTemplate *>::iterator it = templates.begin(); it != templates.end(); ++it) 
	{
		delete *it;
	}
	for (std::vector<ConstraintTemplate *>::iterator it = constraints.begin(); it != constraints.end(); ++it) 
	{
		delete *it;
	}
}

GameObject *ObjectType::Create(GameState &state, const std::vector<vec3> &translation, const std::vector<quat> &rotation, float scale, Color color, Color teamcolor)
{
	GameObject *obj = new GameObject(state.getObjectId(), this, scale, color, teamcolor);
	for (size_t i = 0; i < translation.size(); i++) {
		obj->bones[i].obj = obj;
		templates[i]->create(state.physics, translation[i], rotation[i], &obj->bones[i]);
	}
	return obj;
}

GameObject *ObjectType::Create(GameState &state, const vec3 &translation, const quat &rotation, float scale, Color color, Color teamcolor)
{
	std::vector<vec3> trans;
	std::vector<quat> rots;
	for (size_t i = 0; i < templates.size(); i++) {
		trans[i] = translation + rotation * templates[i]->translation;
		rots[i] = rotation + templates[i]->rotation;
	}
	return Create(state, trans, rots, scale, color, teamcolor);
}

