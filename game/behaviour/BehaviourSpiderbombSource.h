#ifndef BEHAVIOURSPIDERBOMBSOURCE_H
#define BEHAVIOURSPIDERBOMBSOURCE_H

#include "BehaviourType.h"

class SoundFragment;

BEHAVIOUR_CLASS(SpiderbombSource)
{
public:
	BehaviourSpiderbombSource(Object *obj, const std::map<std::string, std::string> &);
	void CreateSpiderBomb();
private:
	SoundFragment *SpiderBombCreated;
};

#endif

