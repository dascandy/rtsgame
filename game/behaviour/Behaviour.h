#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <map>
#include <string>
#include <vector>

class Object;
class Behaviour;
class Vector3;
class Bone;

class trigger {
public:
	trigger(Behaviour *behaviour, void (Behaviour::*func)())
	: behaviour(behaviour)
	, func(func)
	{
	}
	void operator()()
	{
		(behaviour->*func)();
	}
	void (Behaviour::*func)();
	Behaviour *behaviour;
};

class event {
public:
	void Add(trigger *t) 
	{
		triggers.push_back(t); 
	}
	void Remove(trigger *t) 
	{
		for (std::vector<trigger *>::iterator i = triggers.begin(); i != triggers.end(); ++i)
		{
			if (*i == t)
			{
				*i = triggers.back();
				break;
			}
		}
		triggers.pop_back();
	}
	void operator()()
	{
		for (std::vector<trigger *>::iterator i = triggers.begin(); i != triggers.end(); ++i)
		{
			(**i)();
		}
	}
private:
	std::vector<trigger *> triggers;
};

class Behaviour
{
public:
	Behaviour(Object *obj) : obj(obj) {}
	virtual ~Behaviour() {}
	virtual void TryUpdateFrame(const Vector3 &) {}
	virtual void HandleCollision(Bone *, Bone *) {}
	void DefineTrigger(void (Behaviour::*func)())
	{
		triggers.push_back(new trigger(this, func));
	}
	void DefineEvent(event &ev)
	{
		events.push_back(&ev);
	}
	std::vector<event *> events;
	std::vector<trigger *> triggers;
protected:
	Object *obj;
};

class ActiveBehaviour : public Behaviour
{
public:
	ActiveBehaviour(Object *obj, float range);
	virtual void TryUpdateFrame(const Vector3 &location);
	virtual void UpdateFrame() = 0;
	virtual void OutOfRange() {}
protected:
	bool wasInRange;
	float range;
};

#endif

