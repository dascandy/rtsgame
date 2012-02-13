#ifndef BEHAVIOURLIFT_H
#define BEHAVIOURLIFT_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Lift)
{
public:
	BehaviourLift(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	SoundFragment *LiftMove;
	float currentPosition;
	float basePos;
	enum {
		Down,
		MovingUp,
		WaitingUp,
		Up,
		MovingDown,
		WaitingDown,
	} state;
};

#endif

