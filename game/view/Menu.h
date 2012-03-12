#ifndef MENU_H
#define MENU_H

#include "InputCallback.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "ResourceManager.h"
#include "Font.h"
#include "glm.h"
#include "MenuScene.h"
#include "TextureRenderPass.h"

class RenderTarget;
class Menu : public InputCallback
{
public:
	Menu(RenderTarget &rt);
	void create();
	void update(int ms);
	void run();
	virtual void MouseMoveX(int);
	virtual void MouseMoveY(int);
	virtual void MouseScrollUp();
	virtual void MouseScrollDown();
	virtual void MouseButtonUp(int);
	virtual void MouseButtonDown(int);
	virtual void KeyDown(int);
	virtual void KeyUp(int);
private:
	Res<Model> createLogo(float size = 1.0f);
	RenderTarget &rt;
	Res<Font> font;
	MenuScene scene;
	vec3 viewposition;
	mat4 projection, view_proj;
	bool created;
	TextureRenderPass pass;
};

#endif

