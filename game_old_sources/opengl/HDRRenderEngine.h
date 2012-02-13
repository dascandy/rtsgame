#ifndef HDRRENDERENGINE_H
#define HDRRENDERENGINE_H

#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"

class Scene;

class HDRRenderEngine : public RenderPass
{
public:
	enum ToneMapType {
		Linear,
		Reinhard,
	};
	HDRRenderEngine(Texture &input, RenderTarget &output, ToneMapType type);
private:
	void DoRun();
	Texture &input;
/*
	Texture bloomDS, bloomHB, bloomVB;
	TextureRenderPass bloomDSR, bloomHBR, bloomVBR;
	RenderTarget targetDS, targetHB, targetVB;
*/
	TextureRenderPass outputRender;
	RenderTarget &output;

	RenderView view;
};

#endif

