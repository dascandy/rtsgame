#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Color.h"
#include "Light.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "RenderView.h"
class Texture;
class Bone;

class SpotLight : public Light 
{
public:
	bool enabled;
	Color color;
	Vector3 location;
	Quaternion direction;
	float angle;
	RenderView view;
	Texture *lighttexture;
	float brightness;
	Bone *bone;
	SpotLight(Color color, Vector3 location, Quaternion direction, float angle, Texture *lighttexture);
	SpotLight(const SpotLight &other, Bone *bone);
	bool isEnabled();
	void Update();
};

#endif

