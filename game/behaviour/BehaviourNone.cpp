#include "BehaviourNone.h"
#include "Behaviour.h"
#include <assert.h>
#include <stdlib.h>

DEFINE_BEHAVIOUR_TYPE(None, 0, 0)

std::string BehaviourTypeNone::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeNone::getEventName(int)
{
	assert(false); exit(-1);
}

