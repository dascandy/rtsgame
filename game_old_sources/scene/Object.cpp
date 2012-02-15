#include "Object.h"
//#include "Physics.h"
#include "ObjectType.h"
#include "Model.h"
#include "BehaviourType.h"
#include "debug.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "BoneTemplate.h"
#include "Bone.h"
#include "profile.h"

Object::Object(ObjectType &objectType, const std::vector<vec3> &location, const std::vector<quat> &rot)
: objectType(objectType)
, behaviour(0)
, isStatic(true)
{
/*	int bone = 0;
	for (std::vector<BoneTemplate *>::iterator it = objectType.physicsModel->templates.begin(); it != objectType.physicsModel->templates.end(); ++it)
	{
		Bone *newBone = new Bone(location[bone], rot[bone], this, (*it)->generateMass());
		if (newBone->mass != 0) isStatic = false;
		IPhysics::Instance().AddBone(*newBone, **it);
		bones.push_back(newBone);
		bone++;
	}

	for (std::vector<PointLight>::iterator it = objectType.pointLights.begin(); it != objectType.pointLights.end(); ++it)
	{
		PointLight *newLight = new PointLight(*it, bones[0]);
		pointLights.push_back(newLight);
	}
	for (std::vector<SpotLight>::iterator it = objectType.spotLights.begin(); it != objectType.spotLights.end(); ++it)
	{
		SpotLight *newLight = new SpotLight(*it, bones[0]);
		spotLights.push_back(newLight);
	}

	for (std::vector<ConstraintTemplate *>::iterator it = objectType.physicsModel->constraints.begin(); it != objectType.physicsModel->constraints.end(); ++it)
	{
		(*it)->Create(this);
	}
*/
//	IPhysics::Instance().AddObject(*this, objectType);
}

Object::Object(ObjectType &objectType, const vec3 &location, const quat &rot)
: objectType(objectType)
, behaviour(0)
, isStatic(true)
{
/*
	for (std::vector<BoneTemplate *>::iterator it = objectType.physicsModel->templates.begin(); it != objectType.physicsModel->templates.end(); ++it)
	{
		Bone *newBone = new Bone(rot * (*it)->translation + location, rot * (*it)->rotation, this, (*it)->generateMass());
		if (newBone->mass != 0) isStatic = false;
		IPhysics::Instance().AddBone(*newBone, **it);
		bones.push_back(newBone);
	}

	TODO_W("fix up lights so they can be tied to non-bone-0");
	for (std::vector<PointLight>::iterator it = objectType.pointLights.begin(); it != objectType.pointLights.end(); ++it)
	{
		PointLight *newLight = new PointLight(*it, bones[0]);
		pointLights.push_back(newLight);
	}
	for (std::vector<SpotLight>::iterator it = objectType.spotLights.begin(); it != objectType.spotLights.end(); ++it)
	{
		SpotLight *newLight = new SpotLight(*it, bones[0]);
		spotLights.push_back(newLight);
	}

	for (std::vector<ConstraintTemplate *>::iterator it = objectType.physicsModel->constraints.begin(); it != objectType.physicsModel->constraints.end(); ++it)
	{
		IPhysics::Instance().AddConstraint((*it)->Create(this));
	}
*/
//	IPhysics::Instance().AddObject(*this, objectType);
}

Object::Object(const Object &orig)
: objectType(orig.objectType)
, behaviour(orig.objectType.behaviour->Create(this, objectType.args))
, isStatic(false)
{
/*
	std::vector<Bone *>::const_iterator bit = orig.bones.begin();
	for (std::vector<BoneTemplate *>::iterator it = objectType.physicsModel->templates.begin(); it != objectType.physicsModel->templates.end(); ++it, ++bit)
	{
		Bone *newBone = new Bone(**bit, this);
		if (newBone->mass != 0) isStatic = false;
		IPhysics::Instance().AddBone(*newBone, **it);
		bones.push_back(newBone);
	}

	TODO_W("fix up lights so they can be tied to non-bone-0");
	for (std::vector<PointLight>::iterator it = objectType.pointLights.begin(); it != objectType.pointLights.end(); ++it)
	{
		PointLight *newLight = new PointLight(*it, bones[0]);
		pointLights.push_back(newLight);
	}
	for (std::vector<SpotLight>::iterator it = objectType.spotLights.begin(); it != objectType.spotLights.end(); ++it)
	{
		SpotLight *newLight = new SpotLight(*it, bones[0]);
		spotLights.push_back(newLight);
	}
*/
	// patch up the joints
//	IPhysics::Instance().AddObject(*this, objectType);
}

Object::~Object()
{
//	IPhysics::Instance().RemoveObject(*this);
	delete behaviour;
}

void Object::Draw(ShaderProgram &prog) 
{
	Texture *tex = objectType.texture;
	if (!tex) {
		unsigned char val[] = { 255,255,255,255 };
		static Texture tex2(1, 1, GL_RGBA8);
		tex2.SetContent(val);
		tex = &tex2;
	}

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
/*
bool Object::hasLights()
{
	return (pointLights.size() + spotLights.size()) > 0;
}

void Object::GetLights(std::vector<PointLight *> &outPointLights, std::vector<SpotLight *> &outSpotLights)
{
	for (std::vector<PointLight *>::iterator it = pointLights.begin();
		 it != pointLights.end(); ++it)
	{
		if ((*it)->isEnabled())
		{
			(*it)->Update();
			outPointLights.push_back(*it);
		}
	}
	for (std::vector<SpotLight *>::iterator it = spotLights.begin();
		 it != spotLights.end(); ++it)
	{
		if ((*it)->isEnabled())
		{
			(*it)->Update();
			outSpotLights.push_back(*it);
		}
	}
}

*/


