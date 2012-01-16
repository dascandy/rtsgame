#ifndef BEHAVIOURFLAME_H
#define BEHAVIOURFLAME_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Flame)
{
public:
	BehaviourFlame(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	bool enabled;
	int partsperframe;
	Vector3 location;
};

#endif

