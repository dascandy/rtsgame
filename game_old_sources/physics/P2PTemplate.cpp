#include "P2PTemplate.h"
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include "Bone.h"

btTypedConstraint *P2PTemplate::Create(Object *o)
{
	Bone *ba = o->bones[boneA];
	Bone *bb = o->bones[boneB];
	return new btPoint2PointConstraint(*ba->body, *bb->body, btVector3(a.x, a.y, a.z), btVector3(b.x, b.y, b.z));
}

