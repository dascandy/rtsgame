#include "EffectRenderPass.h"
#include "Model.h"

EffectRenderPass::EffectRenderPass(Res<ShaderProgram> program, RenderTarget &target)
: TextureRenderPass(program, Model::Square(), target)
{
}

