
#define _LIB
#define _WINDOWS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include "Physics.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Object.h"
#include "ObjectType.h"
#include "debug.h"
#include "Bone.h"
#include "BoneTemplate.h"

#define BIT(x) (1 << (x))

class Physics : public IPhysics {
private:
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
	void AddObject(Object &obj, ObjectType &model);
	void RemoveObject(Object &obj);
	void AddBone(Bone &object, BoneTemplate &base);
	void RemoveBone(Bone &object);
	void *AddGround(float *ground, int x, int y, float scale);
	void AddConstraint(btTypedConstraint *constraint);
	void ApplyForce(Object &obj, const Vector3 &force);
	void SetVelocity(Object &obj, const Vector3 &velocity);
	Object *PerformRaycast(const Vector3 &location, const Vector3 &delta, Vector3 &hitLocation, Vector3 &normal);
};

class GroundMotionState : public btMotionState {
public:
	GroundMotionState(Vector3) 
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

class MotionState : public btDefaultMotionState {
	btTransform transform;
	Bone &obj;
public:
	MotionState(Bone &object)
		: obj(object)
	{
		transform.setOrigin(btVector3(object.translation.x, object.translation.y, object.translation.z));
	}
	void getWorldTransform(btTransform& worldTrans ) const
	{
		worldTrans = transform;
	}
	void setWorldTransform(const btTransform &worldTrans) 
	{
		transform = worldTrans;
		btVector3 origin = worldTrans.getOrigin();
		obj.translation = Vector3(origin.getX(), origin.getY(), origin.getZ());
		btQuaternion rotation = worldTrans.getRotation();
		obj.rotation = Quaternion(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
		obj.transformation = Matrix(obj.translation, obj.rotation);
		btDefaultMotionState::setWorldTransform(worldTrans);
	}
};

IPhysics &IPhysics::Instance() { static Physics physics; return physics; }

Physics::Physics() 
: maxObjects(10240)
, worldMin(-10000, -10000, -10000)
, worldMax(10000, 10000, 10000)
, broadPhase()
, configuration()
, dispatcher(&configuration)
, solver()
, world(&dispatcher, &broadPhase, &solver, &configuration)
{
	world.setGravity(btVector3(0, -10.0f, 0));
	btCollisionObject *bc = new btCollisionObject();
	bc->setCollisionShape(new btStaticPlaneShape(btVector3(0,1,0), -30));
	world.addCollisionObject(bc);
}

Physics::~Physics() 
{

}

void Physics::AddObject(Object &/*obj*/, ObjectType &/*model*/)
{
}

void Physics::AddBone(Bone &object, BoneTemplate &base)
{
	if (base.shape)
	{
		btVector3 fallInertia;
		base.shape->calculateLocalInertia(object.mass, fallInertia);
		MotionState *motionState = new MotionState(object);
		const Quaternion &rot = object.rotation;
		const Vector3 &trans = object.translation;
		motionState->setWorldTransform(btTransform(btQuaternion(rot.x,rot.y, rot.z, rot.w), btVector3(trans.x, trans.y, trans.z)));
		btRigidBody::btRigidBodyConstructionInfo constructionInfo(object.mass, motionState, base.shape, fallInertia);
		btRigidBody *body = new btRigidBody(constructionInfo);
//		body->setActivationState(0);
		body->setUserPointer(&object);
		world.addRigidBody(body);
		object.body = body;
	}
}

void *Physics::AddGround(float *ground, int x, int y, float scale) {
	btHeightfieldTerrainShape *hts = new btHeightfieldTerrainShape(x, y, ground, scale, -1000, 1000, 1, PHY_FLOAT, false);
	btCollisionObject *bc = new btCollisionObject();
	bc->setCollisionShape(hts);
	world.addCollisionObject(bc);

	return hts;
}

void Physics::AddConstraint(btTypedConstraint *constraint)
{
	world.addConstraint(constraint);
}

void Physics::RemoveObject(Object &obj)
{

}

void Physics::RemoveBone(Bone &object)
{
	btRigidBody *body = (btRigidBody *)object.body;
	world.removeRigidBody(body);
	delete (MotionState *)body->getMotionState();
	delete body;
	object.body = NULL;
}

void Physics::PerformPhysics(float timestep) 
{
	TODO_W("Make this run over all substeps of the simulation");
	world.stepSimulation(timestep, 10, 1.0f / 60.0f);

	int numManifolds = dispatcher.getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = dispatcher.getManifoldByIndexInternal(i);
		Bone* objA = static_cast<Bone *>(static_cast<btCollisionObject*>(contactManifold->getBody0())->getUserPointer());
		Bone* objB = static_cast<Bone *>(static_cast<btCollisionObject*>(contactManifold->getBody1())->getUserPointer());
		if (objA && objB)
		{
			objA->CollideWith(objB);
			objB->CollideWith(objA);
		}
	}
}

void Physics::ApplyForce(Object &obj, const Vector3 &force)
{
	btRigidBody *body = (btRigidBody *)obj.physicsHandle;
	body->applyCentralForce(btVector3(force.x, force.y, force.z));
	body->setActivationState(ACTIVE_TAG);
}

void Physics::SetVelocity(Object &obj, const Vector3 &velocity)
{
	btRigidBody *body = (btRigidBody *)obj.physicsHandle;
	body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

Object *Physics::PerformRaycast(const Vector3 &location, const Vector3 &delta, Vector3 &hitLocation, Vector3 &normal)
{
	btVector3 from(location.x, location.y, location.z);
	Vector3 target = location + delta;
	btVector3 to(target.x, target.y, target.z);
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

    world.rayTest(from, to, rayCallback);

    if (rayCallback.hasHit())
    {
        btRigidBody* body = btRigidBody::upcast( rayCallback.m_collisionObject );
		Object* obj = static_cast<Object *>(body->getUserPointer());

		if (obj) 
		{
			hitLocation = Vector3(rayCallback.m_hitPointWorld.getX(), rayCallback.m_hitPointWorld.getY(), rayCallback.m_hitPointWorld.getZ());
			normal = Vector3(rayCallback.m_hitNormalWorld.getX(), rayCallback.m_hitNormalWorld.getY(), rayCallback.m_hitNormalWorld.getZ()).normalize();
			return obj;
		}
    }
	return NULL;
}

