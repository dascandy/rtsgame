#include "BehaviourCamera.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include <assert.h>
#include "Object.h"
#include "debug.h"
#include <math.h>
#include "Bone.h"

DEFINE_BEHAVIOUR_TYPE(Camera, 0, 1)

std::string BehaviourTypeCamera::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeCamera::getEventName(int no)
{
	switch(no)
	{
	case 0: return "SilencerSpotted";
	default: assert(false); exit(-1);
	};
}

BehaviourCamera::BehaviourCamera(Object *obj, std::map<std::string, std::string> &args)
: ActiveBehaviour(obj, 70)
, movementstate(StallLeft)
, state(Searching)
, minAngle(-71.7f)
, currentAngle(-71.7f)
, currentFrame(0)
, currentFocusFrame(0)
, CameraMoving(soundDatabase["CAMTURN"])
, CameraFocussing(soundDatabase["SERVO80"])
, movingSound(NULL)
{
	DefineEvent(SilencerSpotted);
	if (args["type"] == "quarter")
	{
		maxAngle = -90 - minAngle;
	}
	else if (args["type"] == "half")
	{
		maxAngle = 0 - minAngle;
	}
	else if (args["type"] == "threequarter")
	{
		maxAngle = 90 - minAngle;
	}
	else
	{
		LOG_WARN("Invalid setting for camera behaviour type; %s", args["type"].c_str());
		maxAngle = 0 - minAngle;
	}
}

void BehaviourCamera::UpdateFrame()
{
	float silencerAngle = SilencerAngle();
	switch(state)
	{
	case Tracking:
		if (silencerAngle > -5 && silencerAngle < 5)
		{
			// in view, keep tracking
		}
		else if (silencerAngle > -30 && silencerAngle < 0)
		{
			// silencer off to the left but still in view
			if (movementstate == GoingRight)
				currentFrame = GoingFrameCount - currentFrame;

			if (currentFrame < GoingFrameCount) currentFrame++;
			movementstate = GoingLeft;
			currentAngle = maxAngle - (maxAngle - minAngle) * (1.0f * currentFrame / GoingFrameCount);
		}
		else if (silencerAngle < 30 && silencerAngle > 0)
		{
			// silencer off to the left but still in view
			if (movementstate == GoingLeft)
				currentFrame = GoingFrameCount - currentFrame;

			if (currentFrame < GoingFrameCount) currentFrame++;
			movementstate = GoingRight;
			currentAngle = (maxAngle - minAngle) * (1.0f * currentFrame / GoingFrameCount) + minAngle;
		}
		else
		{
			// silencer out of view, go search
			state = Searching;
			currentFocusFrame = 0;
		}
		break;
	case Focussing:
		currentFocusFrame++;
		if (silencerAngle < -30 || silencerAngle > 30)
		{
			state = Searching;
			movingSound = SoundEngine::Instance().PlaySoundPositionalC(CameraMoving, obj->bones[0]->translation, 1);
			currentFocusFrame = 0;
		}
		else if (currentFocusFrame == FocusFrameCount)
		{
			SilencerSpotted();
			state = Tracking;
		}
		break;
	case Searching:
		currentFrame++;
		switch(movementstate)
		{
		case StallLeft:
			if (currentFrame == StallFrameCount)
			{
				movementstate = GoingRight;
				movingSound = SoundEngine::Instance().PlaySoundPositionalC(CameraMoving, obj->bones[0]->translation, 1);
				currentFrame = 0;
			}
			break;
		case GoingRight:
			currentAngle = (maxAngle - minAngle) * (1.0f * currentFrame / GoingFrameCount) + minAngle;
			if (currentFrame >= GoingFrameCount)
			{
				movementstate = StallRight;
				if (movingSound)
				{
					movingSound->Stop();
					movingSound = NULL;
				}
				currentFrame = 0;
			}
			break;
		case StallRight:
			if (currentFrame == StallFrameCount)
			{
				movementstate = GoingLeft;
				movingSound = SoundEngine::Instance().PlaySoundPositionalC(CameraMoving, obj->bones[0]->translation, 1);
				currentFrame = 0;
			}
			break;
		case GoingLeft:
			currentAngle = maxAngle - (maxAngle - minAngle) * (1.0f * currentFrame / GoingFrameCount);
			if (currentFrame >= GoingFrameCount)
			{
				movementstate = StallLeft;
				if (movingSound)
				{
					movingSound->Stop();
					movingSound = NULL;
				}
				currentFrame = 0;
			}
			break;
		}
		if (silencerAngle > -15 && silencerAngle < 15)
		{
			switch(movementstate)
			{
			case StallLeft: movementstate = GoingLeft; currentFrame = GoingFrameCount; break;
			case StallRight: movementstate = GoingRight; currentFrame = GoingFrameCount; break;
			}
			if (movingSound)
			{
				movingSound->Stop();
				movingSound = NULL;
			}

			SoundEngine::Instance().PlaySoundPositional(CameraFocussing, obj->bones[0]->translation, 2);
			state = Focussing;
		}
	}
	SetCurrentPosition();
}

void BehaviourCamera::OutOfRange()
{
	if (movingSound)
	{
		movingSound->Stop();
		movingSound = NULL;
	}
}

float BehaviourCamera::SilencerAngle()
{
	TODO_W("make silencer angle implementation");
	return 40;
}

static const float PI = 3.1415926535f;

void BehaviourCamera::SetCurrentPosition()
{
	float rotation = (PI * currentAngle) / 360;
	TODO_W("Somehow make camera work again");
//	this->obj->setRotation(Quaternion(cos(rotation), 0, sin(rotation), 0));
}

