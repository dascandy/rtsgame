#ifndef BlendTransitionTarget_H
#define BlendTransitionTarget_H

#include "MainTarget.h"

class PixelShader;
class RenderTarget;

class BlendTransitionTarget : public MainTarget
{
public:
	BlendTransitionTarget(MainTarget *source, MainTarget *target, PixelShader *transitionShader, msec_t duration);
	~BlendTransitionTarget();
	virtual MainTarget *Update(msec_t msecs);
	virtual void draw(RenderTarget *);
private:
	MainTarget *source;
	MainTarget *target;
	PixelShader *transitionShader;
	RenderTarget *sourceRT;
	RenderTarget *targetRT;
	msec_t p, duration;
};

#endif

