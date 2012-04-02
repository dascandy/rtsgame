#ifndef BONE_H
#define BONE_H

#include "glm.h"
#include "debug.h"

class btRigidBody;
class GameObject;

class Bone
{
public:
	glm::vec3 translation;
	glm::quat rotation;
	GameObject *obj;
	float mass;
	btRigidBody *handle;
	Bone() 
	{
	}
	inline void CollideWith(Bone * /*bone*/)
	{
		TODO_W("Add collision handling code");
	}
	friend class BoneMotionState;
	friend class Physics;
};

#endif

