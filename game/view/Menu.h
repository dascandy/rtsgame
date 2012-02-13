#ifndef MENU_H
#define MENU_H

#include "InputCallback.h"
#include "Texture.h"
#include "RenderView.h"
#include "RenderTarget.h"

class RenderTarget;

class Menu : public InputCallback
{
public:
	Menu(RenderTarget &rt);
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
	RenderTarget &rt;
};

#endif


