#include "BehaviourLift.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include <assert.h>
#include "Object.h"
#include "debug.h"
#include <math.h>
#include "Scene.h"
#include "Camera.h"
#include "Bone.h"
#include "GameState.h"

DEFINE_BEHAVIOUR_TYPE(Lift, 0, 0)

static const float UpPosition = 6.0f,
	             DownPosition = 0.2f;

std::string BehaviourTypeLift::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeLift::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourLift::BehaviourLift(Object *obj, std::map<std::string, std::string> &)
: ActiveBehaviour(obj, 70)
, state(Down)
, currentPosition(0.2f)
, basePos(obj->bones[0]->translation.y)
{
	TODO_W("Mix the lift sounds somehow to make this continuously enough");
//	LiftMove = soundDatabase["LIFT"];
}

void BehaviourLift::UpdateFrame()
{
	Vector3 objPos = obj->bones[0]->translation;
	Vector3 crusaderDelta = obj->bones[0]->translation - GameState::Instance().gamePosition;
	crusaderDelta.y = 0;
	if (state == Down || state == Up)
	{
		if (crusaderDelta.len() < 1.5)
		{
			SoundEngine::Instance().PlaySoundPositional(LiftMove, obj->bones[0]->translation, 2);
			if (state == Down)
				state = MovingUp;
			else
				state = MovingDown;
		}
	}
	else if (state == WaitingDown || state == WaitingUp)
	{
		if (crusaderDelta.len() > 2)
		{
			if (state == WaitingDown)
				state = Down;
			else
				state = Up;
		}
	}
	else if (state == MovingUp)
	{
		if (currentPosition < UpPosition)
		{
			currentPosition += 0.05f;
			objPos.y = basePos + currentPosition;
			TODO("Make lift work with forces instead");
//			obj->setTranslation(objPos);
		}
		else
			state = WaitingUp;
	}
	else
	{
		if (currentPosition > DownPosition)
		{
			currentPosition -= 0.05f;
			objPos.y = basePos + currentPosition;
			TODO("Make lift work with forces instead");
//			obj->setTranslation(objPos);
		}
		else
			state = WaitingDown;
	}
}

void BehaviourLift::OutOfRange()
{
	
}

