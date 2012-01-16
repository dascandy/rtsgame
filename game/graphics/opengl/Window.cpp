#include "Window.h"
#include "Scene.h"
#include "Object.h"
#include "Model.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_scancode.h>
#include <windows.h> // only for DebugBreak()
#include "Texture.h"
#include "ParticleEngine.h"
#include "InputCallback.h"
#include "HDRRenderEngine.h"
#include "GameState.h"
#include "profile.h"
#include "debug.h"

extern int num_tris;

Window::Window(int width, int height, bool fullscreen)
: BaseWindow(width, height, fullscreen)
, callback(0)
, rt(width, height)
, isActive(true)
, gview(rt)
#ifndef NO_EDITING
, editmode(false)
, eview(rt)
#endif
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
}

Window::~Window()
{
}

void Window::Resized(int width, int height)
{
	this->width = width;
	this->height = height;
	rt.width = width;
	rt.height = height;
}

void Window::Draw()
{
#ifndef NO_EDITING
	if (editmode)
		eview.Run();
	else
#endif
		gview.Run();

	SDL_GL_SwapWindow(surface);
	profile_print();
}

void Window::Run()
{
	SDL_Event event;

	while (!GameState::Instance().ended)
	{
		InputCallback *callback = &gview;
#ifndef NO_EDITING
		if (editmode)
			callback = &eview;
#endif

		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
#ifndef NO_EDITING
				if (event.key.keysym.scancode == SDL_SCANCODE_E &&
					event.key.keysym.mod == (KMOD_LSHIFT | KMOD_LCTRL))
					editmode = !editmode;
#endif
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

		// run this too when inactive, to avoid a huge burst of data when it does
		GameState::Instance().Update(isActive);
		
		if (isActive)
		{
			Draw();
			printf("%d triangles\n", num_tris);
			num_tris = 0;
		}
	}
}

