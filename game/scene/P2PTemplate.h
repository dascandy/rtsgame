#ifndef P2PTEMPLATE_H
#define P2PTEMPLATE_H

class btTypedConstraint;
class Object;
#include "ConstraintTemplate.h"
#include "glm.h"

class P2PTemplate : public ConstraintTemplate {
public:
	P2PTemplate(size_t boneA, size_t boneB, vec3 a, vec3 b)
		: ConstraintTemplate(boneA, boneB)
		, a(a)
		, b(b)
	{}
	btTypedConstraint *Create(Object *o);
private:
	vec3 a, b;
};

#endif

