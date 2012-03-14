#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "BaseWindow.h"
#include "GameView.h"
#include "Menu.h"
#include "Var.h"
#include "TextureRenderPass.h"

struct SDL_Window;

class Scene;
class InputCallback;

class GameWindow : public BaseWindow
{
public:
	GameWindow(Game *game, int width, int height, bool fullscreen);
	~GameWindow();
	void run();
	void update(int ms);
	InputCallback *callback;
	RenderTarget rt;
	RenderTarget rtMenu;
	RenderTarget rtGameview;
	Menu menu;
	GameView gameView;
	Game *game;
	bool isActive;
	Var<float> *menuOpacity;
	VarNum<int> *time;
	bool inMenu;
	TextureRenderPass merge;
	Res<Texture> tMenu, tGameview;
	void SaveGame(const char *name);
	void LoadGame(const char *name);
private:
	void Draw();
	void Resized(int width, int height);
};

#endif

