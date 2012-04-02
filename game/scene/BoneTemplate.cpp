#include "BoneTemplate.h"
#include "Bone.h"
#include <btBulletDynamicsCommon.h>
#include "Physics.h"

void BoneTemplate::create(Physics &physics, const vec3 &location, const quat &rotation, Bone *bone) 
{
	btVector3 fallInertia;
	shape->calculateLocalInertia(mass, fallInertia);
	BoneMotionState *motionState = new BoneMotionState(*bone);
	motionState->setWorldTransform(btTransform(btQuaternion(rotation.x,rotation.y, rotation.z, rotation.w), btVector3(location.x, location.y, location.z)));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, fallInertia);
	btRigidBody *body = new btRigidBody(constructionInfo);
	body->setUserPointer(bone);
	physics.world.addRigidBody(body);
	bone->mass = mass;
	bone->translation = location;
	bone->rotation = rotation;
	bone->handle = body;
}

