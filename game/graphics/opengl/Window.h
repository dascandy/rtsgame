#ifndef WINDOW_H
#define WINDOW_H

#include "BaseWindow.h"
#include "GameView.h"
#ifndef NO_EDITING
#include "EditView.h"
#endif

struct SDL_Window;

class Scene;
class InputCallback;

class Window : public BaseWindow
{
public:
	Window(int width, int height, bool fullscreen);
	~Window();
	void Run();
	InputCallback *callback;
private:
	void Draw();
	void Resized(int width, int height);
	RenderTarget rt;
	bool isActive;
	GameView gview;
	void *mainContext;
	void *renderContext;
	void *loaderContext;
	void *particleContext;
#ifndef NO_EDITING
	bool editmode;
	EditView eview;
#endif
};

#endif

