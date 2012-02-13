#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

class ShaderProgram;

class RenderView
{
public:
	void Set(ShaderProgram &prog, const std::string &postfix = "");
	glm::vec3 location;
	glm::quat rotation;
	glm::mat4 projection;
};

#endif

