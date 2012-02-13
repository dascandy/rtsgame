#ifndef DEFERREDRENDERENGINE_H
#define DEFERREDRENDERENGINE_H

#include "VertexBuffer.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"
#include "Scene.h"

class DeferredRenderEngine : public RenderPass
{
public:
	DeferredRenderEngine(Scene &scene, RenderTarget &finalTarget, RenderView &finalView);
protected:
	void DoRun();
	Scene &scene;
	RenderTarget &target;
	RenderView &view;
	Texture color, depth, normal, other, *perlin, *random;
	TextureRenderPass initial, ambient;
	RenderTarget initialR;
	RenderView nullView;
};

#endif

