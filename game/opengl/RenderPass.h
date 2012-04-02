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
	virtual const char *getName() = 0;
};

template <typename T>
class TVarSetter : public VarSetter {
private:
	const char *name;
	T *val;
public:
	TVarSetter(const char *name, T *val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name, *val);
	}
	const char *getName() { return name; }
};

template <typename T>
class TFixedVarSetter : public VarSetter {
private:
	const char *name;
	T val;
public:
	TFixedVarSetter(const char *name, T val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name, val);
	}
	const char *getName() { return name; }
};

template <typename T>
class TVarRefSetter : public VarSetter {
private:
	const char *name;
	Var<T> **val;
public:
	TVarRefSetter(const char *name, Var<T> **val) 
		: name(name)
		, val(val)
	{}
	void run(Res<ShaderProgram> &prog) {
		prog->Set(name, ***val);
	}
	const char *getName() { return name; }
};

class GpuRenderPass : public RenderPass
{
public:
	GpuRenderPass(Res<ShaderProgram> prog) : program(prog) {}
	void Run();
	void AddTexture(std::string name, Res<Texture> tex);
	void WipeVal(const char *name);
	template <typename T>
	void Register(const char *name, const T &val) {
		WipeVal(name);
		setters.push_back(new TFixedVarSetter<T>(name, val));
	}
	template <typename T>
	void Register(const char *name, T &val) {
		WipeVal(name);
		setters.push_back(new TVarSetter<T>(name, &val));
	}
	template <typename T>
	void Register(const char *name, Var<T> **val) {
		WipeVal(name);
		setters.push_back(new TVarRefSetter<T>(name, val));
	}
	template <typename T>
	void Set(const char *name, const T &val) {
		WipeVal(name);
		program->SetActive();
		program->Set(name, val);
	}
	Res<ShaderProgram> program;
protected:
	std::vector<std::pair<std::string, Res<Texture> > > textures;
	std::vector<VarSetter *> setters;
};

#endif

