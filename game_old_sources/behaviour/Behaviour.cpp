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

void ActiveBehaviour::TryUpdateFrame(const glm::vec3 &location)
{
	if (length(obj->bones[0]->translation - location) < range)
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

