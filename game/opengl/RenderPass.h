#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include <map>
#include <vector>
#include "Texture.h"

class Renderable;
class Object;

class RenderPass
{
public:
	RenderPass() : enabled(true) {}
	virtual ~RenderPass() {}
	virtual void Run();
	bool enabled;
protected:
	virtual void DoRun() = 0;
};

class GpuRenderPass : public RenderPass
{
public:
	GpuRenderPass(Res<ShaderProgram> prog) : program(prog) {}
	void Run();
	void AddTexture(std::string name, Res<Texture> tex);
protected:
	std::vector<std::pair<std::string, Res<Texture> > > textures;
	Res<ShaderProgram> program;
};

#endif

