#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

//#include "Physics.h"
#include <map>
#include <string>
#include <vector>
class Model;
class Texture;
class BehaviourType;
class Object;
class BoneTemplate;
class PhysicsModel;

class ObjectType {
public:
	ObjectType(Model *model, PhysicsModel *physicsModel, Texture *texture, BehaviourType *behaviour, const std::map<std::string, std::string> &args, float mass, float masssd); 
	Object *Create(const std::vector<vec3> &translation, const std::vector<quat> &rotation);
	Object *Create(const vec3 &translation, const quat &rotation);
	PhysicsModel *physicsModel;
	Model *gfxModel;
	Texture *texture;
	Texture *specularT;
	Texture *emissiveT;
	Texture *normalT;
	BehaviourType *behaviour;
	float mass;
	float masssd;
	std::string name;
	std::map<std::string, std::string> args;
};

#endif

