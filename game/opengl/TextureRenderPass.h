#ifndef TEXTURERENDERPASS_H
#define TEXTURERENDERPASS_H

#include "RenderPass.h"
#include "ShaderProgram.h"

class VertexBuffer;
class RenderTarget;
class RenderView;

class TextureRenderPass : public GpuRenderPass
{
public:
	TextureRenderPass(ShaderProgram program, Renderable &input, RenderTarget &target, RenderView &view);
	void DoRun();
private:
	Renderable &input;
	RenderTarget &target;
	RenderView &view;
};

#endif

