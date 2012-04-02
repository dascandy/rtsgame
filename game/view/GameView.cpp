#include "GameView.h"
#include "Model.h"
#include "GameState.h"
#include "MenuObject.h"

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

GameView::GameView(RenderTarget &target)
: ground(ResourceManager::Instance().getResource<ShaderProgram>("renderground"), Model::TesselatedSquare(128), target)
, water(ResourceManager::Instance().getResource<ShaderProgram>("renderwater"), Model::TesselatedSquare(128), target)
, gameObjects(ResourceManager::Instance().getResource<ShaderProgram>("renderGameObjs"), Model::Square(), target)
, hud(ResourceManager::Instance().getResource<ShaderProgram>("renderHud"), hudscene, target)
, loading(ResourceManager::Instance().getResource<ShaderProgram>("renderLoading"), loadingscene, target)
, state(0)
, target(target)
{
	loadingscene.add(new MenuObject(ResourceManager::Instance().getResource<Model>("loadingbar")));
	loading.Register("model_mvp", ortho( -500.0f, 500.0f, 500.0f * target.height / target.width, -500.0f * target.height / target.width, -5.0f, 5.0f));
	loading.AddTexture("tex", ResourceManager::Instance().getResource<Texture>("progressbar"));
}

void GameView::run()
{
	if (!state) return;

	if (**state->progress < 100) {
		target.Clear();
		loading.Run();
	} else {
		gameObjects.Run();
		ground.Run();
		water.Run();
		hud.Run();
	}
}

void GameView::update(int ms)
{
	if (state)
		state->update(ms);
}

void GameView::setGameState(GameState *newstate) {
	if (state)
		delete state;
	state = newstate;
	loading.Register("progress", &state->progress);
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

