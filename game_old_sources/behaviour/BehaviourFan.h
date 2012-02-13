#ifndef BEHAVIOURFAN_H
#define BEHAVIOURFAN_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"
#include "Quaternion.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Fan)
{
public:
	BehaviourFan(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	SoundFragment *FanChop;
	int currentRotation;
	Quaternion initialRotation;
	Quaternion lastSetRotation;
	static const int fullRotationFrames = 180;
};

#endif

