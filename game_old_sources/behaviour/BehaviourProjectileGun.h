#ifndef BEHAVIOURPROJECTILEGUN_H
#define BEHAVIOURPROJECTILEGUN_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(ProjectileGun)
{
public:
	BehaviourProjectileGun(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	bool enabled;
	int partsperframe;
	Vector3 location;
};

#endif

