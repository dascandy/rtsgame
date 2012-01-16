#include "GameView.h"
#include "debug.h"
#include "Scene.h"
#include "Physics.h"
#include "Object.h"
#include "RenderTarget.h"
#include "Model.h"
#include "profile.h"
#include "Bone.h"

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

GameView::GameView(RenderTarget &target)
: target(target)
, hdrout(target.width, target.height)
, hdrbuf(target.width, target.height, GL_RGBA32F)
, dre(Scene::Instance(), hdrout, gameView)
, hre(hdrbuf, target, HDRRenderEngine::Linear)
{
	hdrout.AddTarget(hdrbuf);
	gameView.projection = Matrix(16.0f/9, 1, 10000);
	gameView.rotation = Quaternion(0, 0, -90);
}

void GameView::Run()
{
	gameView.location = Vector3(0, 5, 0);
	hdrout.Clear();
	dre.Run();
	target.Clear();
	hre.Run();
}

void GameView::MouseMoveX(int)
{

}

void GameView::MouseMoveY(int)
{
	
}

void GameView::MouseScrollUp()
{
	
}

void GameView::MouseScrollDown()
{
	
}

void GameView::MouseButtonUp(int)
{
	
}

void GameView::MouseButtonDown(int)
{
	
}

void GameView::KeyDown(int)
{
}

void GameView::KeyUp(int)
{
	
}

