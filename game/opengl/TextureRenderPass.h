#ifndef TEXTURERENDERPASS_H
#define TEXTURERENDERPASS_H

#include "RenderPass.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"

class VertexBuffer;
class RenderTarget;
class RenderView;

class TextureRenderPass : public GpuRenderPass
{
public:
	TextureRenderPass(Res<ShaderProgram> program, Renderable &input, RenderTarget &target);
	void DoRun();
private:
	Renderable &input;
	RenderTarget &target;
};

#endif

