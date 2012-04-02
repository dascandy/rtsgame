#include "Scene.h"
#include "Object.h"
#include "Model.h"
#include "Bone.h"
#include <algorithm>
#include <debug.h>

Scene::Scene()
: pe(*this)
{
}

void Scene::AddObject(Object *obj)
{
	objects.push_back(obj);
	if (obj->behaviour) 
		activeObjects.push_back(obj);
}

void Scene::SetGround(Ground *ground)
{
	this->ground = ground;
}

void Scene::RemoveObject(Object *obj)
{
	objects.erase(std::find(objects.begin(), objects.end(), obj));
	if (obj->behaviour) 
		activeObjects.erase(std::find(activeObjects.begin(), activeObjects.end(), obj));
}

void Scene::Draw(ShaderProgram &prog)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->Draw(prog);
	}
	ground->Draw(prog);
}

void Scene::DrawStatic(ShaderProgram &prog)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if (!(*it)->behaviour)
			(*it)->Draw(prog);
	}
	ground->Draw(prog);
}

void Scene::DrawDynamicNormal(ShaderProgram &prog)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->behaviour)
			(*it)->Draw(prog);
	}
}

void Scene::DrawParticles(ShaderProgram &prog)
{
	pe.Draw(prog);
}

void Scene::AddParticles(ParticleEngine::Particle *parts, unsigned int num)
{
	pe.AddParticles(parts, num);
}

void Scene::DrawPick(ShaderProgram &prog, std::vector<Object *> &objs)
{
	int n = 0;
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		prog.Set("pick_id", n++);
		objs.push_back(*it);
		(*it)->Draw(prog);
	}
}

void Scene::Update(const Vector3 &location)
{
	TODO_W("Re-add particle engine when performance allows");
//	pe.Update();

	for (std::vector<Object *>::iterator it = activeObjects.begin(); it != activeObjects.end(); ++it)
	{
		(*it)->behaviour->TryUpdateFrame(location);
	}
}

void Scene::GetLights(const Vector3 &/*location*/, std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		pointLights.insert(pointLights.end(), (*it)->pointLights.begin(), (*it)->pointLights.end());
		spotLights.insert(spotLights.end(), (*it)->spotLights.begin(), (*it)->spotLights.end());
	}
}

void Scene::GetObjectsNear(const Vector3 &location, std::vector<Object *> &objects, float distance)
{
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if (((*it)->bones[0]->translation - location).len() < distance) 
			objects.push_back(*it);
	}
}

void Scene::Save() 
{

}

void Scene::Load() 
{

}

