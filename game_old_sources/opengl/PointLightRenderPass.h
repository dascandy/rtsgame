#ifndef POINTLIGHTRENDERPASS_H
#define POINTLIGHTRENDERPASS_H

#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"

class PointLight;
class Scene;

class PointLightRenderPass : public RenderPass
{
public:
	PointLightRenderPass(Texture &depth, Texture &normals, Texture &color, PointLight &light, RenderView &view, RenderTarget &output);
private:
	void DoRun();
	Texture &depth, &normals, &color;
	PointLight &light;
	TextureRenderPass render;
};

#endif

