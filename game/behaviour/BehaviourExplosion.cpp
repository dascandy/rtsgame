#include "BehaviourExplosion.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include "Object.h"
#include "debug.h"
#include "Scene.h"
#include <assert.h>
#include <math.h>
#include "Bone.h"

DEFINE_BEHAVIOUR_TYPE(Explosion, 0, 0)

std::string BehaviourTypeExplosion::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeExplosion::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourExplosion::BehaviourExplosion(Object *obj, std::map<std::string, std::string> &)
: ActiveBehaviour(obj, 70)
, currentFrame(0)
{
	ExplosionSound = soundDatabase["EXPLD10A"];
}

static const float PI = 3.1415926535f;

void BehaviourExplosion::UpdateFrame()
{
	std::vector<Object *> objects;
	currentFrame++;
	switch(currentFrame)
	{
	case 1:
		SoundEngine::Instance().PlaySoundPositional(ExplosionSound, obj->bones[0]->translation, 20);

		Scene::Instance().GetObjectsNear(obj->bones[0]->translation, objects, 15);
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			//TODO: cause explosion damage
		}

		objects.clear();
		Scene::Instance().GetObjectsNear(obj->bones[0]->translation, objects, 15);
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			TODO("apply force to bones instead");
//			Vector3 delta = (*it)->getTranslation() - obj->bones[0]->translation;
//			(*it)->applyForce(delta.normalize() * sqrt(225 - delta.len() * delta.len())*50);
		}
		break;
	case 180:
		Scene::Instance().RemoveObject(obj);
		break;
	}
}

void BehaviourExplosion::OutOfRange()
{
	
}

