#include "BaseWindow.h"
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <stdio.h>

BaseWindow::BaseWindow(int width, int height, bool fullscreen)
: width(width)
, height(height)
, fullscreen(fullscreen)
, surface(0)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init failed\n");
		throw 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int full = (fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	surface = SDL_CreateWindow("blaah", 50, 50, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | full);

	if (!surface)
	{
		printf("SDL_SetVideoMode failed");
		throw 1;
	}

	context = SDL_GL_CreateContext(surface);
	// this patch doesn't appear to be in the sdl 1.3 mainline
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_SHARE_CONTEXT, (intptr_t)context);

	glewInit();
}

BaseWindow::~BaseWindow()
{
	SDL_Quit();
}

