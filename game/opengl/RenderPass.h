#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include <map>
#include <vector>
#include "Texture.h"
#include "Var.h"

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

class VarSetter {
public:
	virtual ~VarSetter() {}
	virtual void run(Res<ShaderProgram> &prog) = 0;
};

template <typename T>
class TVarSetter : public VarSetter {
private:
	std::string name;
	T *val;
public:
	TVarSetter(std::string name, T *val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name.c_str(), *val);
	}
};

template <typename T>
class TFixedVarSetter : public VarSetter {
private:
	std::string name;
	T val;
public:
	TFixedVarSetter(std::string name, T val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name.c_str(), val);
	}
};

template <typename T>
class TVarRefSetter : public VarSetter {
private:
	std::string name;
	Var<T> **val;
public:
	TVarRefSetter(std::string name, Var<T> **val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name.c_str(), ***val);
	}
};

class GpuRenderPass : public RenderPass
{
public:
	GpuRenderPass(Res<ShaderProgram> prog) : program(prog) {}
	void Run();
	void AddTexture(std::string name, Res<Texture> tex);
	template <typename T>
	void Register(std::string name, const T &val) {
		setters.push_back(new TFixedVarSetter<T>(name, val));
	}
	template <typename T>
	void Register(std::string name, T &val) {
		setters.push_back(new TVarSetter<T>(name, &val));
	}
	template <typename T>
	void Register(std::string name, Var<T> **val) {
		setters.push_back(new TVarRefSetter<T>(name, val));
	}
	Res<ShaderProgram> program;
protected:
	std::vector<std::pair<std::string, Res<Texture> > > textures;
	std::vector<VarSetter *> setters;
};

#endif

