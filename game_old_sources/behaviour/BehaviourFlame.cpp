#include "BehaviourFlame.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include "Object.h"
#include "debug.h"
#include "Scene.h"
#include <assert.h>
#include <math.h>

float frand()
{
	return 1.0f - rand() / (0.5f * RAND_MAX);
}

DEFINE_BEHAVIOUR_TYPE(Flame, 0, 0)

std::string BehaviourTypeFlame::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeFlame::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourFlame::BehaviourFlame(Object *obj, std::map<std::string, std::string> &)
: ActiveBehaviour(obj, 64)
, enabled(true)
, partsperframe(50)
{
}

static const float PI = 3.1415926535f;

void BehaviourFlame::UpdateFrame()
{
	if (!enabled)
		return;

	TODO("Fix flame behaviour to work with a bone");
/*
	// TODO: remove VLA hack
	Vector3 baseDir = obj->getRotation() * Vector3(0.005f, 0.005f, 0.15f);
	ParticleEngine::Particle parts[50] = {0};
	for (int i = 0; i < partsperframe; i++)
	{
		parts[i].x = obj->getTranslation().x;
		parts[i].y = obj->getTranslation().y;
		parts[i].z = obj->getTranslation().z;
		parts[i].dx = 0.01f*frand() + baseDir.x;
		parts[i].dy = 0.01f*frand() + baseDir.y;
		parts[i].dz = 0.01f*frand() + baseDir.z;
		parts[i].heat = 3.0f+frand();
		parts[i].ttl = 500;
		parts[i].size = frand() * 0.1f + 0.1f;
		parts[i].pressure = 0;
	};
	Scene::Instance().AddParticles(parts, partsperframe);
	*/
}

void BehaviourFlame::OutOfRange()
{
	
}

