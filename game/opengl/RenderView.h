#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include "glm.h"
#include <string>
#include "ResourceManager.h"

class ShaderProgram;

class RenderView
{
public:
	void Set(Res<ShaderProgram> &prog, const std::string &postfix = "");
	glm::vec3 location;
	glm::quat rotation;
	glm::mat4 projection;
};

#endif

