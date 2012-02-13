#ifndef BEHAVIOURTYPE_H
#define BEHAVIOURTYPE_H

#include <map>
#include <string>

#include "Behaviour.h"

class Object;

class BehaviourType
{
public:
	static std::map<std::string, BehaviourType *> &List()
	{
		static std::map<std::string, BehaviourType *> list;
		return list;
	};
	virtual ~BehaviourType() {}
	virtual int getTriggerCount() = 0;
	virtual std::string getTriggerName(int no) = 0;
	virtual int getEventCount() = 0;
	virtual std::string getEventName(int no) = 0;
	virtual Behaviour *Create(Object *, std::map<std::string, std::string> &) = 0;
};

#define BEHAVIOUR_CLASS(c) class BehaviourType##c : public BehaviourType\
{\
public:\
	BehaviourType##c();\
	virtual int getTriggerCount();\
	virtual std::string getTriggerName(int no);\
	virtual int getEventCount();\
	virtual std::string getEventName(int no);\
	virtual Behaviour *Create(Object *, std::map<std::string, std::string> &);\
};\
class Behaviour##c : public Behaviour

#define ACTIVE_BEHAVIOUR_CLASS(c) class BehaviourType##c : public BehaviourType\
{\
public:\
	BehaviourType##c();\
	virtual int getTriggerCount();\
	virtual std::string getTriggerName(int no);\
	virtual int getEventCount();\
	virtual std::string getEventName(int no);\
	virtual Behaviour *Create(Object *, std::map<std::string, std::string> &);\
};\
class Behaviour##c : public ActiveBehaviour

#define DEFINE_BEHAVIOUR_TYPE(c, numTriggers, numEvents) static BehaviourType##c _instance;\
\
BehaviourType##c::BehaviourType##c()\
{\
	BehaviourType::List().insert(std::make_pair(std::string(#c), this));\
}\
\
int BehaviourType##c::getTriggerCount()\
{\
	return numTriggers;\
}\
\
int BehaviourType##c::getEventCount()\
{\
	return numEvents;\
}\
\
Behaviour *BehaviourType##c::Create(Object *obj, std::map<std::string, std::string> &args)\
{\
	return new Behaviour##c(obj, args);\
}

#endif

