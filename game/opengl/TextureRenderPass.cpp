#include "TextureRenderPass.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "Renderable.h"
#include <GL/glew.h>

TextureRenderPass::TextureRenderPass(ShaderProgram program, Renderable &input, RenderTarget &target, RenderView &view)
: GpuRenderPass(program)
, input(input)
, target(target)
, view(view)
{

}

void TextureRenderPass::DoRun()
{
	target.Activate();
	Use shader(program);
	view.Set(program);
	input.Draw(program);
}

