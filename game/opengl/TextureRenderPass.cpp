#include "TextureRenderPass.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "Renderable.h"
#include <GL/glew.h>

TextureRenderPass::TextureRenderPass(Res<ShaderProgram> program, Renderable &input, RenderTarget &target)
: GpuRenderPass(program)
, input(input)
, target(target)
{

}

void TextureRenderPass::DoRun()
{
	target.Activate();
	program->SetActive();
	input.Draw(program);
}


