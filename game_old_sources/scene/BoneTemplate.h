#ifndef BONETEMPLATE_H
#define BONETEMPLATE_H

#include <glm.h>

class btCollisionShape;

class BoneTemplate
{
public:
	vec3 translation;
	quat rotation;
private:
	btCollisionShape *shape;
	float mass;
	float masssd;
	friend class Physics;
public:
	BoneTemplate(const vec3 &location, const quat &rotation, btCollisionShape *shape, float mass, float masssd)
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

