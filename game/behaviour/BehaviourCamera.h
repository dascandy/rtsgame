#ifndef BEHAVIOURCAMERA_H
#define BEHAVIOURCAMERA_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Camera)
{
public:
	BehaviourCamera(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	float SilencerAngle();
	void SetCurrentPosition();
	enum { StallLeft, GoingRight, StallRight, GoingLeft } movementstate;
	enum { Searching, Focussing, Tracking } state;
	float minAngle;
	float maxAngle;
	float currentAngle;
	int currentFrame;
	int currentFocusFrame;
	SoundFragment *CameraMoving;
	SoundFragment *CameraFocussing;
	SoundRepeatInstance *movingSound;
	event SilencerSpotted;
	static const int StallFrameCount = 60;
	static const int GoingFrameCount = 120;
	static const int FocusFrameCount = 60;
};

#endif

