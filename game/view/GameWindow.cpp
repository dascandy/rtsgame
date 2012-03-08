#include "GameWindow.h"
#include <SDL/SDL.h>
#include <SDL/SDL_scancode.h>
#include "profile.h"

extern int num_tris;

GameWindow::GameWindow(int width, int height, bool fullscreen)
: BaseWindow(width, height, fullscreen)
, callback(0)
, rt(width, height, false)
, menu(rt)
, gameView(rt)
, isActive(true)
, inMenu(false)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
}

GameWindow::~GameWindow()
{
}

void GameWindow::Resized(int width, int height)
{
	this->width = width;
	this->height = height;
	rt.width = width;
	rt.height = height;
}

void GameWindow::Draw()
{
	if (inMenu) 
		menu.run();
	else
		gameView.run();

	SDL_GL_SwapWindow(surface);
}

void GameWindow::run()
{
	SDL_Event event;

	InputCallback *callback = &gameView;

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				throw 0;
			callback->KeyDown(event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			callback->KeyUp(event.key.keysym.scancode);
			break;
		case SDL_MOUSEMOTION:
			callback->MouseMove(event.motion.x, height-event.motion.y-1);
			if (event.motion.xrel) callback->MouseMoveX(event.motion.xrel);
			if (event.motion.yrel) callback->MouseMoveY(event.motion.yrel);
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0)
				callback->MouseScrollUp();
			else if (event.wheel.y > 0)
				callback->MouseScrollDown();

			break;
		case SDL_MOUSEBUTTONDOWN:
			callback->MouseButtonDown(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			callback->MouseButtonUp(event.button.button);
			break;
		case SDL_WINDOWEVENT:
			switch(event.window.event)
			{
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_HIDDEN:
				isActive = false;
				break;
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				isActive = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				Resized(event.window.data1, event.window.data2);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				throw 0;
				break;
			}
			break;
		default:
			printf("Unknown event type %d\n", event.type);
			break;
		}
	}

	if (isActive)
	{
		Draw();
		printf("%d triangles\n", num_tris);
		num_tris = 0;
	}
}

