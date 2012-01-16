#include "BehaviourContactExplosive.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include <assert.h>
#include "Object.h"
#include "debug.h"
#include <math.h>
#include "Scene.h"
#include "Camera.h"
#include "ObjectType.h"
#include "ObjectTypeDatabase.h"

DEFINE_BEHAVIOUR_TYPE(ContactExplosive, 0, 0)

std::string BehaviourTypeContactExplosive::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeContactExplosive::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourContactExplosive::BehaviourContactExplosive(Object *obj, std::map<std::string, std::string> &)
: Behaviour(obj)
, firstCollision(true)
{
}

void BehaviourContactExplosive::HandleCollision(Bone *, Bone *, const Vector3 &location, const Vector3 &)
{
	std::vector<Vector3> locations;
	std::vector<Quaternion> rotations;
	locations.push_back(location);
	rotations.push_back(Quaternion());
	if (firstCollision)
	{
		firstCollision = false;
		Scene::Instance().AddObject(ObjectTypeDatabase::Instance()["explosion"].Create(locations, rotations));
		Scene::Instance().RemoveObject(obj);
	}
}

