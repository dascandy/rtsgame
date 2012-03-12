#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "RenderPass.h"
#include "InputCallback.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "ResourceManager.h"
#include "TextureRenderPass.h"

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
private:
	Res<Texture> bgTex;
	TextureRenderPass trp;
};

#endif

