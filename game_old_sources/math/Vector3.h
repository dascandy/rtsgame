#ifndef VECTOR3
#define VECTOR3

#include <string>

class Vector3
{
public:
	Vector3(float x, float y, float z) : x(raw[0]), y(raw[1]), z(raw[2])
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3() : x(raw[0]), y(raw[1]), z(raw[2])
	{
		x = 0; y = 0; z = 0;
	}
	Vector3(const Vector3 &rhs) : x(raw[0]), y(raw[1]), z(raw[2])
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
	}
	static Vector3 parse(const std::string &arg);
	float raw[3];
	float &x, &y, &z;
	Vector3 &operator=(const Vector3 &rhs) 
	{
		raw[0] = rhs.raw[0];
		raw[1] = rhs.raw[1];
		raw[2] = rhs.raw[2];
		return *this;
	}
	const Vector3 operator*(const Vector3 &rhs) const;	// cross product
	const Vector3 operator*(const float &rhs) const;
	const Vector3 operator/(const float &rhs) const;
	const Vector3 &operator*=(const Vector3 &rhs);
	const Vector3 &operator*=(const float &rhs);
	const Vector3 &operator/=(const float &rhs);
	const Vector3 &operator+=(const Vector3 &rhs);
	const Vector3 operator-() const;
	const Vector3 operator-(const Vector3 &rhs) const;
	const Vector3 operator+(const Vector3 &rhs) const;
	float dotproduct(const Vector3 &rhs) const;
	const Vector3 normalize() const;
	float len() const;
};

#endif

