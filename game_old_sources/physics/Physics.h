#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

class Object;
class ObjectType;
class Vector3;
class Quaternion;
class btCollisionShape;
class btRigidBody;
class Bone;
class BoneTemplate;
class ConstraintTemplate;
class btTypedConstraint;

class PhysicsModel {
public:
	void AddBoneTemplate(BoneTemplate *bt)
	{
		templates.push_back(bt);
	}
	void AddConstraintTemplate(ConstraintTemplate *ct)
	{
		constraints.push_back(ct);
	}
	std::vector<BoneTemplate *> templates;
	std::vector<ConstraintTemplate *> constraints;
};

typedef btRigidBody PhysicsObject;

class IPhysics {
public:
	static IPhysics &Instance();
	virtual void PerformPhysics(float timestep) = 0;
	virtual void AddObject(Object &bone, ObjectType &model) = 0;
	virtual void RemoveObject(Object &obj) = 0;
	virtual void AddBone(Bone &object, BoneTemplate &base) = 0;
	virtual void RemoveBone(Bone &object) = 0;
	virtual void AddConstraint(btTypedConstraint *constraint) = 0;
	virtual void *AddGround(float *ground, int x, int y, float scale) = 0;
/*
	virtual void ApplyForce(Object &obj, const Vector3 &force) = 0;
	virtual void SetVelocity(Object &obj, const Vector3 &velocity) = 0;
	*/
	virtual Object *PerformRaycast(const Vector3 &location, const Vector3 &delta, Vector3 &hitLocation, Vector3 &normal) = 0;
};

#endif

