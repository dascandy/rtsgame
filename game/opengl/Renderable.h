#ifndef RENDERABLE_H
#define RENDERABLE_H

class ShaderProgram;
#include "ResourceManager.h"

class Renderable
{
public:
	virtual void Draw(Res<ShaderProgram> &prog) = 0;
};

#endif

