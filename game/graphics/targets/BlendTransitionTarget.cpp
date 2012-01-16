#include "BlendTransitionTarget.h"
#include "RenderTarget.h"
#include "PixelShader.h"

BlendTransitionTarget::BlendTransitionTarget(MainTarget *source, MainTarget *target, PixelShader *transitionShader, msec_t duration)
: source(source)
, target(target)
, transitionShader(transitionShader)
, sourceRT(0)
, targetRT(0)
, p(0)
, duration(duration)
{
}

BlendTransitionTarget::~BlendTransitionTarget()
{
	delete sourceRT;
	delete targetRT;
}

MainTarget *BlendTransitionTarget::Update(msec_t msecs)
{
	source->Update(msecs);
	target->Update(msecs);
	p += msecs;
	if (p > duration)
		return target;

	return this;
}

void BlendTransitionTarget::draw(RenderTarget *out)
{
	if (!sourceRT)
		sourceRT = new RenderTarget(out->width, out->height);

	if (!targetRT)
		targetRT = new RenderTarget(out->width, out->height);

	source->draw(sourceRT);
	target->draw(targetRT);
	
	transitionShader->set();
	out->setRT(0);
	sourceRT->setTex(0);
	targetRT->setTex(1);
	transitionShader->SetFloat("transitionProgress", (float)p / duration);
}

