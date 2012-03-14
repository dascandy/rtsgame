#include "RenderPass.h"
#include "Texture.h"

void RenderPass::Run() 
{ 
	if (!enabled) return;

	DoRun(); 
}

void GpuRenderPass::Run() 
{ 
	if (!enabled) return;
	
	program->SetActive();
	program->curtex = 0;
	for (std::vector<std::pair<std::string, Res<Texture> > >::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		it->second->Bind(program, it->first);
	}

	for (std::vector<VarSetter *>::iterator it = setters.begin(); it != setters.end(); ++it) {
		(*it)->run(program);
	}

	DoRun(); 

	for (;program->curtex > 0; )
	{
		program->curtex--;
		glActiveTexture(GL_TEXTURE0+program->curtex);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GpuRenderPass::AddTexture(std::string name, Res<Texture> tex)
{
	textures.push_back(make_pair(name, tex));
}

