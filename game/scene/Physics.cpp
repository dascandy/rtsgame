#include "Physics.h"

#define _LIB
#define _WINDOWS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

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

	// This is an emergency stop for objects falling
	btCollisionObject *bc = new btCollisionObject();
	bc->setCollisionShape(new btStaticPlaneShape(btVector3(0,1,0), -30));
	world.addCollisionObject(bc);
}

Physics::~Physics() 
{

}

/*
void *Physics::AddGround(float *ground, int x, int y, float scale) {
	btHeightfieldTerrainShape *hts = new btHeightfieldTerrainShape(x, y, ground, scale, -1000, 1000, 1, PHY_FLOAT, false);
	btCollisionObject *bc = new btCollisionObject();
	bc->setCollisionShape(hts);
	world.addCollisionObject(bc);

	return hts;
}
*/
void Physics::AddConstraint(btTypedConstraint *constraint)
{
	world.addConstraint(constraint);
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
/*
void Physics::ApplyForce(Object &obj, const Vector3 &force)
{
	btRigidBody *body = (btRigidBody *)obj.physicsHandle;
	body->applyCentralForce(btVector3(force.x, force.y, force.z));
	body->setActivationState(ACTIVE_TAG);
}
*/
/*
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
*/

BoneMotionState::BoneMotionState(Bone &object)
	: obj(object)
{
}

void BoneMotionState::getWorldTransform(btTransform& worldTrans ) const
{
	btTransform transform;
	transform.setOrigin(btVector3(obj.translation.x, obj.translation.y, obj.translation.z));
	transform.setRotation(btQuaternion(obj.rotation.x, obj.rotation.y, obj.rotation.z, obj.rotation.w));
	worldTrans = transform;
}

void BoneMotionState::setWorldTransform(const btTransform &worldTrans) 
{
	btVector3 origin = worldTrans.getOrigin();
	obj.translation = vec3(origin.getX(), origin.getY(), origin.getZ());
	btQuaternion rotation = worldTrans.getRotation();
	obj.rotation = quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
	btDefaultMotionState::setWorldTransform(worldTrans);
}
