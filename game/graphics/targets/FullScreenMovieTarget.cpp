#include "FullScreenMovieTarget.h"
#include "debug.h"
#include "Screen.h"
#include "RenderTarget.h"

FullScreenMovieTarget::FullScreenMovieTarget(MovieTexture *movie, MainTarget *targetAfterMovie)
: movie(movie)
, targetAfterMovie(targetAfterMovie)
, cancelled(false)
{
}

MainTarget *FullScreenMovieTarget::Update(msec_t msecs)
{
	if (cancelled)
		return targetAfterMovie;

	if (!movie->Update(msecs))
		return targetAfterMovie;

	return this;
}

void FullScreenMovieTarget::KeyDown(int key)
{
	if (key == SDLK_ESCAPE)
	{
		cancelled = true;
	}
}

void FullScreenMovieTarget::draw(RenderTarget *target)
{
	target->setRT(0);
	Screen::Instance().DrawFullscreenMovieTexture(movie);
}

