#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Vector3.h"

class IFrustum {
public:
	virtual bool getModelVisible(const Vector3 &modelLocation, float radius) const = 0;
};

// Cone from location, into direction, with distance farPlane. Using the nearPlane is optional.
class ConeFrustum : public IFrustum {
public:
	ConeFrustum(const Vector3 &location, const Vector3 &direction, float nearPlane, float farPlane);
	virtual bool getModelVisible(const Vector3 &modelLocation, float radius) const;
	Vector3 location;
	Vector3 direction;
	float nearPlane;
	float farPlane;
};

// (Axis-Aligned) cube that has a simple matching. Used in ortho projection.
class CubeFrustum : public IFrustum {
public:
	CubeFrustum(const Vector3 &center, const Vector3 &size);
	virtual bool getModelVisible(const Vector3 &modelLocation, float radius) const;
	Vector3 location;
	Vector3 size;
};

// Sphere match, used for point lights.
class SphereFrustum : public IFrustum {
public:
	SphereFrustum(const Vector3 &center, float size);
	virtual bool getModelVisible(const Vector3 &modelLocation, float radius) const;
	Vector3 location;
	float size;
};

// Yes match, always says that it matches. Used for directional and ambient lights.
class YesFrustum : public IFrustum {
public:
	YesFrustum() {}
	virtual bool getModelVisible(const Vector3 &modelLocation, float radius) const;
};

#endif

