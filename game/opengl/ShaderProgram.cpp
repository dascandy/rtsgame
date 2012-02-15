#include "ShaderProgram.h"
#include <GL/glew.h>
#include <stdio.h>
#include "Color.h"
#include <debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

ShaderProgram::ShaderProgram(const char *vsh, const char *gsh, const char *fsh, const char **invars, const char **varyings)
: vs(0)
, gs(0)
, fs(0)
, prog(glCreateProgram())
, usage(new int)
{
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
	}
}
/*
void ShaderProgram::Set(const char *name, const Matrix &mat, bool invert)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	if (invert)
	{
		Matrix inv = mat;
		inv.Invert();
		glUniformMatrix4fv(uniform, 1, GL_FALSE, inv.matrix);
	}
	else
	{
		glUniformMatrix4fv(uniform, 1, GL_FALSE, mat.matrix);
	}
}
*/
void ShaderProgram::Set(const char *name, int value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	glUniform1i(uniform, value);
}

void ShaderProgram::Set(const char *name, unsigned int value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	glUniform1ui(uniform, value);
}

bool ShaderProgram::SetTexture(const char *name, int value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return false;

	glUseProgram(prog);
	glUniform1i(uniform, value);
	return true;
}

void ShaderProgram::Set(const char *name, float value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	glUniform1f(uniform, value);
}

void ShaderProgram::Set(const char *name, const glm::vec3 &value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	glUniform3f(uniform, value.x, value.y, value.z);
}

void ShaderProgram::Set(const char *name, const Color &value)
{
	int uniform = glGetUniformLocation(prog, name);
	if (uniform == -1) return;

	glUseProgram(prog);
	glUniform3f(uniform, value.r, value.g, value.b);
}

Use::Use(ShaderProgram &prog) 
{
	glUseProgram(prog.prog); 
//	GameState::Instance().SetVars(prog);
}

Use::~Use() 
{
	glUseProgram(0); 
}

