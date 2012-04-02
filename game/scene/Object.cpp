#include "Object.h"
#include "Model.h"
#include "debug.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Bone.h"
#include "profile.h"

Object::Object(ObjectType &objectType)
: isStatic(true)
{
}

Object::Object(const Object &orig)
: objectType(orig.objectType)
, isStatic(false)
{
}

Object::~Object()
{
}

void Object::Draw(ShaderProgram &prog) 
{
	Texture *tex = objectType.texture;

	int i = 0;
	for (std::vector<Bone *>::iterator it = bones.begin(); it != bones.end(); ++it, ++i)
	{
		char name[20];
		sprintf(name, "mat_model[%d]", i);
		prog.Set(name, (*it)->transformation);
	}
	bool bound = tex->Bind(prog, "modeltex");
	bool boundS = objectType.specularT->Bind(prog, "speculartex");
	bool boundN = objectType.normalT->Bind(prog, "normaltex");
	bool boundE = objectType.emissiveT->Bind(prog, "emissivetex");
	objectType.gfxModel->Draw(prog);

	if (bound) prog.curtex--;
	if (boundS) prog.curtex--;
	if (boundN) prog.curtex--;
	if (boundE) prog.curtex--;
}

