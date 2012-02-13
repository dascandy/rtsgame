#ifndef SPOTLIGHTRENDERPASS_H
#define SPOTLIGHTRENDERPASS_H

#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"
#include "Scene.h"

class SpotLight;

class SpotLightRenderPass : public RenderPass
{
public:
	SpotLightRenderPass(Scene &scene, Texture &depth, Texture &normals, Texture &color, Texture *perlin, Texture *random, SpotLight &light, RenderView &view, RenderTarget &output, bool shadowCasting, bool dynamicObject);
	int step;
private:
	void DoRun();
	Texture &depth, &normals, &color;
	Texture *perlin, *random;
	RenderView &view;
	RenderView noneView;
	RenderTarget &output;
	SpotLight &light;
	TextureRenderPass render;
	OnlyStatic onlyStatic;
	OnlyDynamic onlyDynamic;
	OnlyParticles onlyParticles;

	RenderTarget renderStaticT, renderDynamicT, renderBlur1T, renderBlur2T;
	TextureRenderPass renderStatic, renderDynamic, renderParticles, renderBlur1, renderBlur2;
	Texture smap_static, smap_dynamic, smap_blur1, smap_blur2;

	bool shadowCasting;
	bool dynamicObject;
	bool staticDone;
};

#endif

