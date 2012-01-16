#include "BehaviourProjectileGun.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include "Object.h"
#include "debug.h"
#include "Scene.h"
#include <assert.h>
#include <math.h>

DEFINE_BEHAVIOUR_TYPE(ProjectileGun, 0, 0)

std::string BehaviourTypeProjectileGun::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeProjectileGun::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourProjectileGun::BehaviourProjectileGun(Object *obj, std::map<std::string, std::string> &)
: ActiveBehaviour(obj, 64)
, enabled(true)
, partsperframe(50)
{
}

static const float PI = 3.1415926535f;

void BehaviourProjectileGun::UpdateFrame()
{
	if (!enabled)
		return;
}

void BehaviourProjectileGun::OutOfRange()
{
	
}

