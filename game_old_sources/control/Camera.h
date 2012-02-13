#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Camera(const Vector3 &vec);
	Vector3 getTranslation() const;
	void GetRowMajor(float *matrix) const;

	Vector3 baseLocation;
	float pitch;
	float yaw; // step one: without yaw
};

#endif

