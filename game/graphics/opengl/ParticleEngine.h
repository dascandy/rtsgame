#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include "VertexBuffer.h"
#include "RenderTarget.h"
#include "Texture3d.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"

class Scene;

class ParticleEngine : public Renderable
{
public:
	ParticleEngine(Scene &scene);
	struct Particle {
		float x, y, z;
		float dx, dy, dz;
		float heat;
		float ttl;
		float size;
		float pressure;
	};
	void AddParticles(Particle *p, int count);
	void Update();
	void SetCenterPosition(const Vector3 &location);
	void Draw(ShaderProgram &sp);
private:
	Scene &scene;
	Vector3 centerLocation;
	Texture3d sceneout, vboout, i1, i2;
	RenderTarget threed_scene, threed_vbo, threed_render;
	VertexBuffer in, out;
	RenderView localview;
	TextureRenderPass vboRender;
//	TextureRenderPass sceneRender;
	TextureRenderPass threedrender;
	FeedbackTransformPass vboTransform;
};

#endif

