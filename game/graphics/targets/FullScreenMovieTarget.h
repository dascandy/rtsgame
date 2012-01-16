#ifndef FULLSCREENMOVIETARGET_H
#define FULLSCREENMOVIETARGET_H

#include "MainTarget.h"

class FullScreenMovieTarget : public MainTarget
{
	Texture *tex;
	MainTarget *targetAfterMovie;
	bool cancelled;
	void *currentFrame;
	Movie *movie;
public:
	FullScreenMovieTarget(Movie *movie, MainTarget *targetAfterMovie);
	virtual MainTarget *Update(msec_t msecs);
	virtual void draw(RenderTarget *);
	void KeyDown(int key);
};

#endif

