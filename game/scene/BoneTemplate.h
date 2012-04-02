#ifndef BONETEMPLATE_H
#define BONETEMPLATE_H

#include <glm.h>

class btCollisionShape;
class Bone;

class BoneTemplate
{
public:
	vec3 translation;
	quat rotation;
	float mass;
private:
	btCollisionShape *shape;
	friend class Physics;
public:
	BoneTemplate(const vec3 &location, const quat &rotation, btCollisionShape *shape, float mass)
	: translation(location)
	, rotation(rotation)
	, shape(shape)
	, mass(mass)
	{
	}
	void create(Physics &physics, const vec3 &location, const quat &rotation, Bone *bone);
};

#endif

