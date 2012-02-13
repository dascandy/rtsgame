#include "Quaternion.h"
#include "Vector3.h"
#include <math.h>

Quaternion::Quaternion()
: w(1)
, x(0)
, y(0)
, z(0)
{
}

#define TORADIANS(x) ((x * 3.1415926f) / 180)

Quaternion::Quaternion(float pitch, float yaw, float roll)
{
	const float SinPitch = sin(TORADIANS(pitch*0.5f));
	const float CosPitch = cos(TORADIANS(pitch*0.5f));
	const float SinYaw = sin(TORADIANS(yaw*0.5f));
	const float CosYaw = cos(TORADIANS(yaw*0.5f));
	const float SinRoll = sin(TORADIANS(roll*0.5f));
	const float CosRoll = cos(TORADIANS(roll*0.5f));
	x = SinRoll * CosPitch * CosYaw - CosRoll * SinPitch * SinYaw;
	y = CosRoll * SinPitch * CosYaw + SinRoll * CosPitch * SinYaw;
	z = CosRoll * CosPitch * SinYaw - SinRoll * SinPitch * CosYaw;
	w = CosRoll * CosPitch * CosYaw + SinRoll * SinPitch * SinYaw;
}

Quaternion::Quaternion(float w, float x, float y, float z)
: w(w)
, x(x)
, y(y)
, z(z)
{
}

bool Quaternion::operator!=(const Quaternion &a) const
{
	return (w != a.w ||
			x != a.x ||
			y != a.y ||
			z != a.z);
}

Quaternion Quaternion::getConjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

Vector3 Quaternion::operator* (const Vector3 &vec) const
{
	Vector3 vn(vec);
 
	Quaternion vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;
 
	return (Vector3(resQuat.x, resQuat.y, resQuat.z));
}

Quaternion Quaternion::operator* (const Quaternion &rq) const
{
	// the constructor takes its arguments as (x, y, z, w)
	return Quaternion(w * rq.w - x * rq.x - y * rq.y - z * rq.z,
					  w * rq.x + x * rq.w + y * rq.z - z * rq.y,
	                  w * rq.y + y * rq.w + z * rq.x - x * rq.z,
	                  w * rq.z + z * rq.w + x * rq.y - y * rq.x);
}

Quaternion &Quaternion::operator*=(const Quaternion &rq)
{
	return *this = *this * rq;
}

