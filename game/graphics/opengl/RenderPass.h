#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include "ShaderProgram.h"
#include <map>
#include <vector>

class Texture;
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
	GpuRenderPass(ShaderProgram prog) : program(prog) {}
	void Run();
	void AddTexture(std::string name, Texture &tex);
//protected:
	std::vector<std::pair<std::string, Texture *> > textures;
	ShaderProgram program;
};

#endif

