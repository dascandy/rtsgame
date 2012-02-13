#include "Camera.h"
#include <math.h>

Camera::Camera(const Vector3 &vec)
: baseLocation(vec)
, pitch(180)
{
}

Vector3 Camera::getTranslation() const
{
	return baseLocation;
}

#define TORADIANS(x) (x / 57.17f)

void Camera::GetRowMajor(float *matrix) const
{
	matrix[0] = cos(TORADIANS(pitch));
	matrix[1] = 0;
	matrix[2] = -sin(TORADIANS(pitch));
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = sin(TORADIANS(pitch));
	matrix[9] = 0;
	matrix[10] = cos(TORADIANS(pitch));
	matrix[11] = 0;
	matrix[12] = -baseLocation.x * cos(TORADIANS(pitch)) - baseLocation.z * sin(TORADIANS(pitch));
	matrix[13] = -baseLocation.y - 1;
	matrix[14] = baseLocation.x * sin(TORADIANS(pitch)) - baseLocation.z * cos(TORADIANS(pitch)) + 5;
	matrix[15] = 1;
}

