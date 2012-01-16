#ifndef Scene_H
#define Scene_H

#include "Vector3.h"
#include <vector>
#include <map>
#include <set>
#include "PointLight.h"
#include "SpotLight.h"
#include "Renderable.h"
#include "ParticleEngine.h"

class Object;
class Ground;

class Scene : public Renderable {
public:
	Scene();
	static Scene &Instance() { static Scene instance; return instance; }
	void events(unsigned int milliseconds);

	void AddObject(Object *);
	void RemoveObject(Object *);
	void AddParticles(ParticleEngine::Particle *part, unsigned int num);
	void MoveObject(Object *, const Vector3 &location);
	void GetLights(const Vector3 &location, std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights);
	void GetObjectsNear(const Vector3 &location, std::vector<Object *> &objects, float distance);
	void Draw(ShaderProgram &prog);
	void DrawStatic(ShaderProgram &prog);
	void DrawDynamicNormal(ShaderProgram &prog);
	void DrawDynamicEffect(ShaderProgram &prog);
	void DrawParticles(ShaderProgram &prog);
	void DrawPick(ShaderProgram &prog, std::vector<Object *> &objs);
	void Update(const Vector3 &location);
	void Save();
	void Load();
	void SetGround(Ground *ground);
private:
	Ground *ground;

	std::vector<Object *> activeObjects;
	std::vector<Object *> objects;
	ParticleEngine pe;
};

class OnlyStatic : public Renderable {
	Scene &scene;
public:
	OnlyStatic(Scene &scene) : scene(scene) {}
	void Draw(ShaderProgram &prog) {
		scene.DrawStatic(prog);
	}
};

class OnlyDynamic : public Renderable {
	Scene &scene;
public:
	OnlyDynamic(Scene &scene) : scene(scene) {}
	void Draw(ShaderProgram &prog) {
		scene.DrawDynamicNormal(prog);
	}
};

class OnlyParticles: public Renderable {
	Scene &scene;
public:
	OnlyParticles(Scene &scene) : scene(scene) {}
	void Draw(ShaderProgram &prog) {
		scene.DrawParticles(prog);
	}
};

#endif

