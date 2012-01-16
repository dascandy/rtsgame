#ifndef MOVIE_H
#define MOVIE_H

#include "os.h"

class Movie
{
public:
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
	virtual void *Frame(msec_t delta) = 0;
	virtual void mix(signed short *target, int sampleCount) = 0;
};

#endif

