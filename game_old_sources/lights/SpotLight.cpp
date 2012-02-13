#include "SpotLight.h"
#include "debug.h"
#include "Quaternion.h"
#include "Bone.h"

SpotLight::SpotLight(Color color, Vector3 location, Quaternion direction, float angle, Texture *lighttexture)
: enabled(true)
, color(color)
, location(location)
, direction(direction)
, angle(angle)
, lighttexture(lighttexture)
, brightness(1.0f)
{
	view.location = location;
	view.rotation = direction;
	view.projection = Matrix(1, 1, 1000);
}

SpotLight::SpotLight(const SpotLight &other, Bone *bone)
: enabled(other.enabled)
, color(other.color)
, location(other.location)
, direction(other.direction)
, angle(other.angle)
, lighttexture(other.lighttexture)
, brightness(other.brightness)
, bone(bone)
{
}

bool SpotLight::isEnabled()
{
	return enabled;
}

void SpotLight::Update()
{
	view.location = bone->rotation * location + bone->translation;
	view.rotation = bone->rotation * direction;
}

