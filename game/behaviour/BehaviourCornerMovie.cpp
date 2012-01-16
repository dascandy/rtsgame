#include "BehaviourCornerMovie.h"
#include "Behaviour.h"
#include "SoundFragment.h"
#include "SoundFragmentDatabase.h"
#include "SoundEngine.h"
#include <assert.h>
#include "Object.h"
#include "debug.h"
#include <math.h>
#include "Scene.h"
#include "Silencer.h"
#include "Camera.h"
#include "MainTarget.h"
#include "Texture.h"
#include "VfwAvi.h"
#include "FullScreenMovieTarget.h"

DEFINE_BEHAVIOUR_TYPE(CornerMovie, 0, 0)

std::string BehaviourTypeCornerMovie::getTriggerName(int)
{
	assert(false); exit(-1);
}

std::string BehaviourTypeCornerMovie::getEventName(int)
{
	assert(false); exit(-1);
}

BehaviourCornerMovie::BehaviourCornerMovie(Object *obj, std::map<std::string, std::string> &args)
: ActiveBehaviour(obj, atoi(args["radius"].c_str()))
, movie(new MovieTexture(new VfwAvi(args["movie"].c_str())))
, startedPlaying(false)
{
}

void BehaviourCornerMovie::UpdateFrame()
{
	if (!startedPlaying)
	{
		MainTarget::currentMainTarget = new FullScreenMovieTarget(movie, MainTarget::currentMainTarget);
		startedPlaying = true;
	}
}

void BehaviourCornerMovie::OutOfRange()
{
	
}

