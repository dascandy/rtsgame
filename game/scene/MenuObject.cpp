#include "MenuObject.h"
#include "Model.h"
#include "debug.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "profile.h"
#include "glm.h"

MenuObject::MenuObject(Res<Model> model, const glm::vec3 &location, const glm::quat &rot)
: model(model)
, location(location)
, rot(rot)
, color(new VarNum<Color>(Color(1, 1, 1, 0)))
, OnActivate(0)
{

}

MenuObject::~MenuObject()
{
}

void MenuObject::Draw(Res<ShaderProgram> &prog) 
{
	glm::mat4 mat_model = mat4_cast(rot);
    mat_model[3] = glm::vec4(location, 1.0);
	prog->Set("mat_model", mat_model);
	prog->Set("color", **color);
	model->Draw(prog);
}

