#ifndef BEHAVIOURNONE_H
#define BEHAVIOURNONE_H

#include "BehaviourType.h"

BEHAVIOUR_CLASS(None)
{
public:
	BehaviourNone(Object *obj, const std::map<std::string, std::string> &) 
		: Behaviour(obj)
	{}
};

#endif

