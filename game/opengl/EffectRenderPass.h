#ifndef EFFECTRENDERPASS_H
#define EFFECTRENDERPASS_H

#include "TextureRenderPass.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"

class VertexBuffer;
class RenderTarget;
class RenderView;

class EffectRenderPass : public TextureRenderPass
{
public:
	EffectRenderPass(Res<ShaderProgram> program, RenderTarget &target);
};

#endif

