#include "GameView.h"
#include "Model.h"
#include "GameState.h"

static const char *invecs[] = 
{
	"in_loc",
	"in_normal",
	"in_tex0",
	"in_arg",
	0
};

GameView::GameView(RenderTarget &target)
: bgTex(ResourceManager::Instance().getResource<Texture>("heightmap"))
, trp(ResourceManager::Instance().getResource<ShaderProgram>("test"), Model::Square(), target)
, state(0)
{
	trp.AddTexture("heightmap", bgTex);
}

void GameView::run()
{
	trp.Run();
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
	trp.AddTexture("heightmap", state->texture);
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

