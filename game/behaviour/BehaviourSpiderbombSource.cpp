#include "BehaviourSpiderbombSource.h"
#include "Behaviour.h"
#include "ObjectTypeDatabase.h"
#include "SoundEngine.h"
#include "SoundFragmentDatabase.h"
#include "Object.h"
#include "ObjectType.h"
#include <assert.h>
#include "debug.h"
#include "Scene.h"
#include "Bone.h"

DEFINE_BEHAVIOUR_TYPE(SpiderbombSource, 1, 0)

BehaviourSpiderbombSource::BehaviourSpiderbombSource(Object *obj, const std::map<std::string, std::string> &) 
: Behaviour(obj)
, SpiderBombCreated(soundDatabase["JETSON1A"])
{
	DefineTrigger((void (Behaviour::*)())&BehaviourSpiderbombSource::CreateSpiderBomb);
}

std::string BehaviourTypeSpiderbombSource::getTriggerName(int no)
{
	if (no == 0) return "CreateSpiderBomb";
	assert(false); exit(-1);
}

std::string BehaviourTypeSpiderbombSource::getEventName(int)
{
	assert(false); exit(-1);
}

void BehaviourSpiderbombSource::CreateSpiderBomb()
{
	std::vector<Vector3> locations;
	std::vector<Quaternion> rotations;
	locations.push_back(obj->bones[0]->translation);
	rotations.push_back(Quaternion());
	Object *spider = ObjectTypeDatabase::Instance()["spiderbomb"].Create(locations, rotations);

	SoundEngine::Instance().PlaySoundPositional(SpiderBombCreated, obj->bones[0]->translation, 2);
	Scene::Instance().AddObject(spider);
}

