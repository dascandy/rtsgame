#ifndef MENUOBJECT_H
#define MENUOBJECT_H

#include "Renderable.h"
#include "glm.h"
#include <vector>
#include "ResourceManager.h"
#include "Model.h"
#include "Var.h"
#include "Color.h"
class ShaderProgram;
class MenuAction;

class MenuObject : public Renderable
{
public:
	MenuObject(Res<Model> model, const vec3 &location = vec3(), const quat &rot = quat());
	~MenuObject();
	void Draw(Res<ShaderProgram> &vs);
	glm::vec3 location;
	glm::quat rot;
	bool clickable;
	MenuAction *OnActivate;
	Var<Color> *color;
	Res<Model> model;
private:
	MenuObject(const MenuObject &);
	MenuObject &operator=(const MenuObject &);
};

#endif

