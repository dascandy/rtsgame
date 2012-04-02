#ifndef CONSTRAINTTEMPLATE_H
#define CONSTRAINTTEMPLATE_H

#include <stddef.h>

class btTypedConstraint;
class Object;

class ConstraintTemplate {
public:
	ConstraintTemplate(size_t boneA, size_t boneB)
	: boneA(boneA)
	, boneB(boneB)
	{
	}
	virtual ~ConstraintTemplate() {}
	virtual btTypedConstraint *Create(Object *o) = 0;
protected:
	size_t boneA, boneB;
};

#endif

