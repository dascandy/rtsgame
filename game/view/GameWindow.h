#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "BaseWindow.h"
#include "GameView.h"
#include "Menu.h"

struct SDL_Window;

class Scene;
class InputCallback;

class GameWindow : public BaseWindow
{
public:
	GameWindow(int width, int height, bool fullscreen);
	~GameWindow();
	void run();
	InputCallback *callback;
	RenderTarget rt;
	Menu menu;
	GameView gameView;
	bool isActive;
	bool inMenu;
private:
	void Draw();
	void Resized(int width, int height);
};

#endif

