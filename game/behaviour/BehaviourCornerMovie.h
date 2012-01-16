#ifndef BEHAVIOURLIFT_H
#define BEHAVIOURLIFT_H

#include "BehaviourType.h"
#include "Behaviour.h"

class MovieTexture;

ACTIVE_BEHAVIOUR_CLASS(CornerMovie)
{
public:
	BehaviourCornerMovie(Object *obj, std::map<std::string, std::string> &args);
	void UpdateFrame();
	void OutOfRange();
private:
	MovieTexture *movie;
	bool startedPlaying;
};

#endif

