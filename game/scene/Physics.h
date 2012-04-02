#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "Physics.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Object.h"
#include "ObjectType.h"
#include "debug.h"
#include "Bone.h"
#include "BoneTemplate.h"

class Object;
class ObjectType;
class btCollisionShape;
class btRigidBody;
class Bone;
class BoneTemplate;
class ConstraintTemplate;
class btTypedConstraint;

class GroundMotionState : public btMotionState {
public:
	GroundMotionState() 
	{
	}
 	void getWorldTransform(btTransform &worldTrans) const 
	{
		worldTrans = btTransform();
	}
	void setWorldTransform(const btTransform &) 
	{
		assert(false);
	}
};

class BoneMotionState : public btDefaultMotionState {
	Bone &obj;
public:
	BoneMotionState(Bone &object);
	void getWorldTransform(btTransform& worldTrans ) const;
	void setWorldTransform(const btTransform &worldTrans);
};

typedef btRigidBody PhysicsObject;

class Physics {
public:
	unsigned short maxObjects;
	btVector3 worldMin, worldMax;
	btDbvtBroadphase broadPhase;
	btDefaultCollisionConfiguration configuration;
	btCollisionDispatcher dispatcher;
	btSequentialImpulseConstraintSolver solver;
	btDiscreteDynamicsWorld world;
public:
	Physics();
	~Physics();
	void PerformPhysics(float timestep);
/*	void *AddGround(float *ground, int x, int y, float scale);
*/	void AddConstraint(btTypedConstraint *constraint);
//	Object *PerformRaycast(const Vector3 &location, const Vector3 &delta, Vector3 &hitLocation, Vector3 &normal);
};

#endif

