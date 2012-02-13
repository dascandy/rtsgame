#include "PointLight.h"
#include "debug.h"
#include "Texture.h"
#include "Bone.h"

PointLight::PointLight(Color color, Vector3 location)
: enabled(true)
, color(color)
, location(location)
, brightness(1.0f)
{
	view.location = location;
}

bool PointLight::isEnabled()
{
	return enabled;
}

void PointLight::Update()
{
	view.location = bone->rotation * location + bone->translation;
}

PointLight::PointLight(const PointLight &pl, Bone *b)
: enabled(pl.enabled)
, color(pl.color)
, location(pl.location)
, brightness(pl.brightness)
, bone(b)
{
	
}

