#include "LookAroundCamera.h"
#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include "Scene.h"

LookAroundCamera::LookAroundCamera() 
: up(0)
, angle(-135)
, xdelta(0)
, ydelta(0)
{
	view.projection = Matrix(1.33f, 1, 10000);
	view.location = Vector3(0, 7, 0);
	Update();
}

void LookAroundCamera::KeyDown(int key)
{
	switch(key)
	{
	case SDL_SCANCODE_W:
		ydelta = -1;
		break;
	case SDL_SCANCODE_S:
		ydelta = 1;
		break;
	case SDL_SCANCODE_A:
		xdelta = -1;
		break;
	case SDL_SCANCODE_D:
		xdelta = 1;
		break;
	}
}

void LookAroundCamera::KeyUp(int key)
{
	switch(key)
	{
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_S:
		ydelta = 0;
		break;
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_D:
		xdelta = 0;
		break;
	}
}

void LookAroundCamera::MouseMoveX(int delta)
{
	angle -= delta;
}

void LookAroundCamera::MouseMoveY(int delta)
{
	up -= delta;
	if (up > 85) up = 85;
	if (up < -85) up = -85;
}

void LookAroundCamera::Update()
{
	view.rotation = Quaternion(angle, 0, 0) * Quaternion(0, 0, up);
	float radangle = angle / 57.17f;
	float radup = up / 57.17f;
	Vector3 movement = Vector3((ydelta * sin(radangle) + xdelta * cos(radangle)) * cos(radup),
							   -ydelta * sin(radup), 
							   (ydelta * cos(radangle) - xdelta * sin(radangle)) * cos(radup));
	view.location += Vector3(movement.x, movement.y, movement.z);
}

