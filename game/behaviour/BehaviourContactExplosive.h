#ifndef BEHAVIOURCONTACTEXPLOSIVE_H
#define BEHAVIOURCONTACTEXPLOSIVE_H

#include "BehaviourType.h"
#include "Behaviour.h"
#include "SoundInstance.h"

class SoundFragment;

BEHAVIOUR_CLASS(ContactExplosive)
{
public:
	BehaviourContactExplosive(Object *obj, std::map<std::string, std::string> &args);
	void HandleCollision(Bone *mine, Bone *his, const Vector3 &location, const Vector3 &);
private:
	bool firstCollision;
};

#endif

