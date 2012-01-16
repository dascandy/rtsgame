#include "BehaviourFan.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include <assert.h>
#include "Object.h"
#include "debug.h"
#include <math.h>
#include "Bone.h"

DEFINE_BEHAVIOUR_TYPE(Fan, 0, 0)

std::string BehaviourTypeFan::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeFan::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourFan::BehaviourFan(Object *obj, std::map<std::string, std::string> &)
: ActiveBehaviour(obj, 70)
, currentRotation(0)
, initialRotation()
, lastSetRotation(initialRotation)
{
	FanChop = soundDatabase["FAN11"];
}

static const float PI = 3.1415926535f;

void BehaviourFan::UpdateFrame()
{
#ifndef NO_EDITING
	if (obj->selected) return;
#endif
	TODO_W("make sound a derived property");
	if (currentRotation * 4 / fullRotationFrames !=
		(currentRotation+1)*4/fullRotationFrames)
	{
		SoundEngine::Instance().PlaySoundPositional(FanChop, obj->bones[0]->translation, 1);
	}
	currentRotation = (currentRotation + 1) % fullRotationFrames;
	float rotation = (PI * currentRotation) / fullRotationFrames;
	// allow editing
	if (obj->bones[0]->rotation != lastSetRotation)
	{
		initialRotation = obj->bones[0]->rotation;
	}

	lastSetRotation = initialRotation * Quaternion(cos(rotation), 0, 0, sin(rotation));
	TODO_W("make fan work with angular rotation force instead");
//	this->obj->setRotation(lastSetRotation);
}

void BehaviourFan::OutOfRange()
{
	
}

