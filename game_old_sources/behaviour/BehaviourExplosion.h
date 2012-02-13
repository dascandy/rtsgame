#ifndef BEHAVIOUREXPLOSION_H
#define BEHAVIOUREXPLOSION_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Explosion)
{
public:
	BehaviourExplosion(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	SoundFragment *ExplosionSound;
	int currentFrame;
};

#endif

