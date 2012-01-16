#ifndef BEHAVIOURPULSATINGLIGHT_H
#define BEHAVIOURPULSATINGLIGHT_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(PulsatingLight)
{
public:
	BehaviourPulsatingLight(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	bool enabled;
	int step;
	int totalsteps;
};

#endif

