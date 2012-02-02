#include "BehaviourPulsatingLight.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include "Object.h"
#include "debug.h"
#include "Scene.h"
#include <assert.h>
#include <math.h>

DEFINE_BEHAVIOUR_TYPE(PulsatingLight, 0, 0)

std::string BehaviourTypePulsatingLight::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypePulsatingLight::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourPulsatingLight::BehaviourPulsatingLight(Object *obj, std::map<std::string, std::string> &args)
: ActiveBehaviour(obj, 128)
, enabled(true)
, step(0)
, totalsteps(120)
{
	if (args.find("totalsteps") != args.end()) 
	{
		totalsteps = atoi(args["totalsteps"].c_str());
	}
}

static const float PI = 3.1415926535f;

void BehaviourPulsatingLight::UpdateFrame()
{
	if (!enabled)
		return;

	step++;
	for (std::vector<PointLight *>::iterator it = obj->pointLights.begin(); it != obj->pointLights.end(); ++it)
	{
		(*it)->brightness = std::max((float)sin(2 * PI * step / totalsteps) * 0.6f + 0.4f, 0.0f);
	}
}

void BehaviourPulsatingLight::OutOfRange()
{
	
}

