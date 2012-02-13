#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include "Physics.h"
#include <map>
#include <string>
#include <vector>
#include "PointLight.h"
#include "SpotLight.h"
class Model;
class Texture;
class BehaviourType;
class Object;
class BoneTemplate;

class ObjectType {
public:
	ObjectType(Model *model, PhysicsModel *physicsModel, Texture *texture, BehaviourType *behaviour, const std::map<std::string, std::string> &args, float mass, float masssd); 
	Object *Create(const std::vector<Vector3> &translation, const std::vector<Quaternion> &rotation);
	Object *Create(const Vector3 &translation, const Quaternion &rotation);
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
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	static Texture *dSpecularT;
	static Texture *dEmissiveT;
	static Texture *dNormalT;
};

#endif

