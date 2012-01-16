#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Resource.h"

class ShaderProgram;

class Renderable : public Resource
{
public:
	virtual void Draw(ShaderProgram &prog) = 0;
};

#endif

