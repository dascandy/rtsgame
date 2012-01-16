#ifndef LOOKAROUNDCAMERA_H
#define LOOKAROUNDCAMERA_H

#include "RenderView.h"
#include "Matrix.h"

class LookAroundCamera
{
public:
	LookAroundCamera();
	void KeyDown(int key);
	void KeyUp(int key);
	void MouseMoveX(int delta);
	void MouseMoveY(int delta);
	void Update();
	Matrix viewMat;
	RenderView view;
	float up;
	float angle;
	int xdelta, ydelta;
};

#endif

