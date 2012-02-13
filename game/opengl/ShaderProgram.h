#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <exception>

class Matrix;
class Vector3;
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
	ShaderProgram(const char *vsh, const char *gsh, const char *fsh, const char **invars, const char **varyings = 0);
	ShaderProgram(const ShaderProgram &other);
	void Set(const char *name, const Matrix &mat, bool invert = false);
	void Set(const char *name, int value);
	void Set(const char *name, unsigned int value);
	void Set(const char *name, float value);
	void Set(const char *name, const Vector3 &value);
	void Set(const char *name, const Color &value);
	bool SetTexture(const char *name, int value);
	~ShaderProgram();
	int curtex;
private:
	int *usage;
	int vs, fs, gs;
	int prog;
	friend class Use;
};

class Use {
public:
	Use(ShaderProgram &prog);
	~Use();
};

#endif

