#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Color.h"
#include "Light.h"
#include "Vector3.h"
#include "RenderView.h"
class Quaternion;
class Texture;
class Bone;

class PointLight : public Light 
{
public:
	bool enabled;
	Color color;
	Vector3 location;
	RenderView view;
	float brightness;
	Bone *bone;
	PointLight(Color color, Vector3 location);
	PointLight(const PointLight &pl, Bone *b);
	bool isEnabled();
	void Update();
};

#endif

