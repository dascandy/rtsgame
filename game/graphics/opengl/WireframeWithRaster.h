#ifndef WIREFRAMEWITHRASTER_H
#define WIREFRAMEWITHRASTER_H

#include "VertexBuffer.h"
#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"

class Scene;

class WireframeWithRaster : public RenderPass
{
public:
	enum Side { Left, Top, Front };
	WireframeWithRaster(Renderable &scene, RenderTarget &finalTarget, Side side);
	float scale;
	Vector3 location;
	void DrawPick(std::vector<Object *> &objs);
protected:
	void DoRun();
	Renderable &scene;
	RenderTarget &target;
	TextureRenderPass raster, objects, obj_ids;
	RenderView view;
};

#endif

