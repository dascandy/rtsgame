#ifndef BASEWINDOW_H
#define BASEWINDOW_H

struct SDL_Window;

class BaseWindow
{
public:
	BaseWindow(int width, int height, bool fullscreen);
	~BaseWindow();
protected:
	int width, height;
	bool fullscreen;
	SDL_Window *surface;
	void *context;
};

#endif

