#include "ShaderProgram.h"
#include <GL/glew.h>
#include <stdio.h>
#include "Color.h"
#include <debug.h>
#include "glm.h"

std::map<std::string, ShaderProgram *> ShaderProgram::shaders;

static inline void compileShader(int prog, int &shader, const char *source, int type)
{
	shader = glCreateShader(type);
	glAttachShader(prog, shader);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	int ok = true;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::string errors;
		errors.reserve(length);
		glGetShaderInfoLog(shader, length, NULL, &errors[0]);
		Fatal(errors.c_str());
		throw ShaderCompileException(errors);
	}
}

ShaderProgram::ShaderProgram(const char *vsh, const char *gsh, const char *fsh, const char **invars, const char **varyings, const char *buffer, const char *name)
: vs(0)
, gs(0)
, fs(0)
, prog(glCreateProgram())
, usage(new int)
, name(name)
, buffer(buffer)
, vsh(vsh)
, gsh(gsh)
, fsh(fsh)
, invars(invars)
, varyings(varyings)
{
	shaders[name] = this;
	*usage = 1;
	if (vsh) compileShader(prog, vs, vsh, GL_VERTEX_SHADER);
	if (fsh) compileShader(prog, fs, fsh, GL_FRAGMENT_SHADER);
	if (gsh) compileShader(prog, gs, gsh, GL_GEOMETRY_SHADER);

	int invarcount = 0;
	while (invars[invarcount]) invarcount++;
	for (int i = 0; i < invarcount; i++)
	{
		glBindAttribLocation(prog, i, invars[i]);
	}

	if (varyings) 
	{
		int count = 0;
		while (varyings[count])
			count++;

		if (count)
			glTransformFeedbackVaryings(prog, count, varyings, GL_INTERLEAVED_ATTRIBS);
	}

	int ok = false;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok)
	{
		int length = 0;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
		std::string errors;
		errors.reserve(length);
		glGetProgramInfoLog(prog, length, NULL, &errors[0]);
		Fatal(errors.c_str());
		throw ShaderCompileException(errors);
	}
}

ShaderProgram::ShaderProgram(const ShaderProgram &other)
: vs(other.vs)
, gs(other.gs)
, fs(other.fs)
, prog(other.prog)
, usage(other.usage)
, name(other.name)
, buffer(other.buffer)
, vsh(other.vsh)
, gsh(other.gsh)
, fsh(other.fsh)
, invars(other.invars)
, varyings(other.varyings)
{
	(*usage)++;
}

ShaderProgram::~ShaderProgram()
{
	(*usage)--;
	if (!(*usage))
	{
		if (prog) glDeleteProgram(prog);
		if (vs) glDeleteShader(vs);
		if (gs) glDeleteShader(gs);
		if (fs) glDeleteShader(fs);
		delete usage;
		shaders.erase(name);
		delete [] buffer;
	}
}

static int usedProg = 0;

void ShaderProgram::Set(const char *name, const mat4 &mat)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::Set(const char *name, const vec2 &vec)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform2fv(uniform, 1, &vec[0]);
}

void ShaderProgram::Set(const char *name, int value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform1i(uniform, value);
}

void ShaderProgram::Set(const char *name, unsigned int value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform1ui(uniform, value);
}

bool ShaderProgram::SetTexture(const char *name, int value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return false;

	glUniform1i(uniform, value);
	return true;
}

void ShaderProgram::Set(const char *name, float value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform1f(uniform, value);
}

void ShaderProgram::Set(const char *name, const glm::vec3 &value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform3f(uniform, value.x, value.y, value.z);
}

void ShaderProgram::Set(const char *name, const Color &value)
{
	if (usedProg != 0 && usedProg != prog) {
		TODO("Fix bug!");
	}
	int uniform = getUniformLocation(name);
	if (uniform == -1) return;

	glUniform3f(uniform, value.r, value.g, value.b);
}

void ShaderProgram::SetActive() 
{
	usedProg = prog;
	glUseProgram(prog); 
}

int ShaderProgram::getUniformLocation(const char *name) {
	std::map<std::string, int>::iterator it = uniforms.find(name);
	if (it != uniforms.end()) return it->second;
	int value = uniforms[name] = glGetUniformLocation(prog, name);
	return value;
}

