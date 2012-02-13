#ifndef BONE_H
#define BONE_H

#include "Vector3.h"
#include "Quaternion.h"
#include "Object.h"
#include "Behaviour.h"

class btRigidBody;

class Bone
{
public:
	Vector3 translation;
	Quaternion rotation;
	Matrix transformation;
	btRigidBody *body;
	Object *obj;
	float mass;
	Bone(const Vector3 &location, const Quaternion &rotation, Object *obj, float mass)
		: translation(location)
		, rotation(rotation)
		, transformation(location, rotation)
		, body(0)
		, obj(obj)
		, mass(mass)
	{
	}
	Bone(Bone &other, Object *newObj)
		: translation(other.translation)
		, rotation(other.rotation)
		, transformation(translation, rotation)
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

