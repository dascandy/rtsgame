#ifndef BONETEMPLATE_H
#define BONETEMPLATE_H

#include "Vector3.h"
#include "Quaternion.h"

class btCollisionShape;

class BoneTemplate
{
public:
	Vector3 translation;
	Quaternion rotation;
private:
	btCollisionShape *shape;
	float mass;
	float masssd;
	friend class Physics;
public:
	BoneTemplate(const Vector3 &location, const Quaternion &rotation, btCollisionShape *shape, float mass, float masssd)
	: translation(location)
	, rotation(rotation)
	, shape(shape)
	, mass(mass)
	, masssd(masssd)
	{
	}
	float generateMass() 
	{
		return mass;
	}
};

#endif

