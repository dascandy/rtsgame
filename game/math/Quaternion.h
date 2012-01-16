#ifndef QUATERNION_H
#define QUATERNION_H

class Vector3;

class Quaternion {
public:
	float w, x, y, z;
	Quaternion();
	Quaternion(float pitch, float yaw = 0, float roll = 0);
	Quaternion(float w, float x, float y, float z);
	Quaternion getConjugate() const;
	Vector3 operator* (const Vector3 &vec) const;
	Quaternion operator* (const Quaternion &rq) const;
	Quaternion &operator*=(const Quaternion &rq);
	bool operator!=(const Quaternion &a) const;
	void draw() const;
};

#endif

