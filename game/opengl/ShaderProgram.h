#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <exception>
#include "glm.h"
#include <map>

class Color;

class ShaderCompileException : public std::exception
{
public:
	ShaderCompileException(const std::string &errors)
		: errors(errors)
	{}
	~ShaderCompileException() throw() {}
	const char *what() const throw() { return errors.c_str(); }
private:
	const std::string errors;
};

class ShaderProgram
{
public:
	static std::map<std::string, ShaderProgram *> shaders;
	static const char *getDirName() { return "shaders"; }
	ShaderProgram(const char *vsh, const char *gsh, const char *fsh, const char **invars, const char **varyings, const char *buffer, const char *name);
	ShaderProgram(const ShaderProgram &other);
	void Set(const char *name, const mat4 &mat);
	void Set(const char *name, const vec2 &vec);
	void SetActive();
	void Set(const char *name, int value);
	void Set(const char *name, unsigned int value);
	void Set(const char *name, float value);
	void Set(const char *name, const vec3 &value);
	void Set(const char *name, const Color &value);
	bool SetTexture(const char *name, int value);
	~ShaderProgram();
	int curtex;
private:
	int getUniformLocation(const char *name);
	int *usage;
	int vs, fs, gs;
	int prog;
	const char *vsh, *gsh, *fsh, **invars, **varyings;
	const char *buffer;
	friend class ShaderProgramHandler;
	friend class ShaderProgramStorer;
	friend class ShaderWebClient;
	std::string name;
	std::map<std::string, int> uniforms;
};

#endif

