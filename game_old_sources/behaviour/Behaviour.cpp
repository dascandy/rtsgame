#include "Behaviour.h"
#include "Object.h"
#include "debug.h"
#include "Bone.h"

ActiveBehaviour::ActiveBehaviour(Object *obj, float range)
: Behaviour(obj)
, wasInRange(false)
, range(range)
{
}

void ActiveBehaviour::TryUpdateFrame(const Vector3 &location)
{
	if ((obj->bones[0]->translation - location).len() < range)
	{
		UpdateFrame();
		wasInRange = true;
	}
	else if (wasInRange)
	{
		OutOfRange();
		wasInRange = false;
	}
}

