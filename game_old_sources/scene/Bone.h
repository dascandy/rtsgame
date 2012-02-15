#ifndef BONE_H
#define BONE_H

#include "glm.h"
#include "Object.h"
#include "Behaviour.h"

class btRigidBody;

class Bone
{
public:
	glm::vec3 translation;
	glm::quat rotation;
	btRigidBody *body;
	Object *obj;
	float mass;
	Bone(const glm::vec3 &location, const glm::quat &rotation, Object *obj, float mass)
		: translation(location)
		, rotation(rotation)
		, body(0)
		, obj(obj)
		, mass(mass)
	{
	}
	Bone(Bone &other, Object *newObj)
		: translation(other.translation)
		, rotation(other.rotation)
		, body(0)
		, obj(newObj)
		, mass(other.mass)
	{
	}
	inline void CollideWith(Bone *bone)
	{
		if (this->obj->behaviour != NULL) 
			this->obj->behaviour->HandleCollision(this, bone); 
	}
	friend class MotionState;
	friend class Physics;
};

#endif

