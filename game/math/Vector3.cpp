#include "Vector3.h"
#include "math.h"
#include <iostream>
#include <vector>
#include "stringutil.h"
#include <stdlib.h>

Vector3 Vector3::parse(const std::string &arg)
{
	std::vector<std::string> items = split(arg, '/');
	return Vector3(float(atof(items[0].c_str())), float(atof(items[1].c_str())), float(atof(items[2].c_str())));
}

const Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

const Vector3 Vector3::operator-(const Vector3 &rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

const Vector3 Vector3::operator+(const Vector3 &rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

const Vector3 &Vector3::operator*=(const Vector3 & rhs)
{
	*this = ((*this) * rhs);
	return *this;
}

const Vector3 &Vector3::operator*=(const float & rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

const Vector3 &Vector3::operator/=(const float &rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

const Vector3 Vector3::operator*(const float &rhs) const
{
	return Vector3(x*rhs, y*rhs, z*rhs);
}

const Vector3 Vector3::operator/(const float &rhs) const
{
	return Vector3(x/rhs, y/rhs, z/rhs);
}

const Vector3 &Vector3::operator+=(const Vector3 & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

const Vector3 Vector3::operator*(const Vector3 &rhs) const
{
	Vector3 rv(y * rhs.z - rhs.y * z,
	           x * rhs.z - rhs.x * z,
	           x * rhs.y - rhs.x * y);
	return rv;
}

float Vector3::dotproduct(const Vector3 &rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z) / len() / rhs.len();
}

float Vector3::len() const
{
	return sqrt(x*x + y*y + z*z);
}

const Vector3 Vector3::normalize() const
{
	float length = len();
	if (length == 0) return Vector3(0, 0, 0);
	Vector3 rv(x / length, y / length, z / length);
	return rv;
}

