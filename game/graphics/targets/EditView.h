#ifndef EDITVIEW_H
#define EDITVIEW_H
#ifndef NO_EDITING

#include "RenderPass.h"
#include "InputCallback.h"
#include "DeferredRenderEngine.h"
#include "LookAroundCamera.h"
#include "TextureRenderPass.h"
#include "WireframeWithRaster.h"
#include "SelectionRectangle.h"
#include "Vector3.h"
#include "HDRRenderEngine.h"
#include <set>

class RenderTarget;
class Object;

class EditView : public InputCallback
{
public:
	EditView(RenderTarget &target);
	virtual void MouseMove(int x, int y);
	virtual void MouseMoveX(int);
	virtual void MouseMoveY(int);
	virtual void MouseScrollUp();
	virtual void MouseScrollDown();
	virtual void MouseButtonUp(int);
	virtual void MouseButtonDown(int);
	virtual void KeyDown(int);
	virtual void KeyUp(int);
	void Run();
private:
	virtual Object *Pick(int x, int y);
	virtual std::set<Object *> PickArea(int x, int y, int x2, int y2);
	void MoveSelection(int dx, int dy);
	void RotateSelection(int dx);
	void DuplicateSelection();
	void AlignSelection();
	RenderTarget &mainTarget;
	RenderTarget tl, tr, bl, br, right;
	WireframeWithRaster left, top, front;
	TextureRenderPass rasterToBr;
	TextureRenderPass dre_tex;
	DeferredRenderEngine dre;
	SelectionRectangle select;
	LookAroundCamera cam;
	bool viewmode;
	int mousex, mousey;
	float scale;
	Vector3 deltaXMovement, deltaYMovement;
	Vector3 rotAxis;
public:
	Vector3 rasterLocation;
private:
	unsigned int catNameIdx;
	std::vector<std::string> catNames;
	unsigned int objNameIdx;
	std::vector<std::string> objNames;
};

#endif

#endif

