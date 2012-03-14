#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "RenderPass.h"
#include "InputCallback.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "ResourceManager.h"
#include "TextureRenderPass.h"

class GameState;

class GameView : public InputCallback
{
public:
	GameView(RenderTarget &target);
	virtual void MouseMoveX(int);
	virtual void MouseMoveY(int);
	virtual void MouseScrollUp();
	virtual void MouseScrollDown();
	virtual void MouseButtonUp(int);
	virtual void MouseButtonDown(int);
	virtual void KeyDown(int);
	virtual void KeyUp(int);
	void run();
	void update(int ms);
	void setGameState(GameState *newstate);
private:
	Res<Texture> bgTex;
	TextureRenderPass trp;
	GameState *state;
};

#endif

