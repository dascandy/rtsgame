#ifndef SELECTIONRECTANGLE_H
#define SELECTIONRECTANGLE_H

#include "RenderTarget.h"
#include "RenderView.h"
#include "TextureRenderPass.h"
#include "FeedbackTransformPass.h"
#include "Texture.h"

class Scene;

class SelectionRectangle : public RenderPass
{
public:
	SelectionRectangle(RenderTarget &output);
	float x, y, x2, y2;
private:
	void DoRun();
	RenderTarget output;
	TextureRenderPass selectionRender;
	RenderView view;
};

#endif

