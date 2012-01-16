#include "Frustum.h"

ConeFrustum::ConeFrustum(const Vector3 &location, const Vector3 &direction, float nearPlane, float farPlane)
: location(location)
, direction(direction)
, nearPlane(nearPlane)
, farPlane(farPlane)
{
}

CubeFrustum::CubeFrustum(const Vector3 &center, const Vector3 &size)
: location(center)
, size(size)
{
}

SphereFrustum::SphereFrustum(const Vector3 &center, float size)
: location(center)
, size(size)
{
}

bool ConeFrustum::getModelVisible(const Vector3 &modelLocation, float radius) const
{
	return true;
}

bool CubeFrustum::getModelVisible(const Vector3 &modelLocation, float radius) const
{
	return true;
}

bool SphereFrustum::getModelVisible(const Vector3 &modelLocation, float radius) const
{
	if ((modelLocation - location).len() > size + radius)
		return false;

	return true;
}

bool YesFrustum::getModelVisible(const Vector3 &modelLocation, float radius) const
{
	return true;
}

