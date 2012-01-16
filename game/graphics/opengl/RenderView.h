#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include "Matrix.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>

class ShaderProgram;

class RenderView
{
public:
	void Set(ShaderProgram &prog, const std::string &postfix = "");
	Vector3 location;
	Quaternion rotation;
	Matrix projection;
};

#endif

