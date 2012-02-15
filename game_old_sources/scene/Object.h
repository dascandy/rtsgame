#ifndef OBJECT_H
#define OBJECT_H

#include "Renderable.h"
#include "glm.h"
#include <vector>
//#include "Physics.h"
class Behaviour;
class Bone;
class IFrustum;
class ObjectType;
class ShaderProgram;

class Object : public Renderable
{
private:
	friend class ObjectType;
	Object(ObjectType &objectType, const std::vector<glm::vec3> &location, const std::vector<glm::quat> &rot);
	Object(ObjectType &objectType, const glm::vec3 &location, const glm::quat &rot);
public:
	Object(const Object &orig);
	~Object();
	void Draw(ShaderProgram &vs);
/*	bool hasLights();
	void GetLights(std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights);
	PhysicsObject *physicsHandle;
*/
protected:
	ObjectType &objectType;
public:
	Behaviour *behaviour;
	bool isStatic;
	std::vector<Bone *> bones;
/*
	std::vector<PointLight *> pointLights;
	std::vector<SpotLight *> spotLights;
*/
private:
	Object &operator=(const Object &);
	friend class MotionState;
};

#endif

