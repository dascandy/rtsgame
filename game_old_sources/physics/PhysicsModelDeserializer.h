#ifndef PHYSICSMODELDESERIALIZER_H
#define PHYSICSMODELDESERIALIZER_H

#include "Physics.h"
#include "stream.h"

class ConstraintTemplate;

class PhysicsModelDeserializer
{
public:
	PhysicsModelDeserializer(std::string name);
	PhysicsModel *deserialize();
private:
	btCollisionShape *deserializeShape();
	btCollisionShape *parseSimpleShape(std::vector<std::string> &items);
	ConstraintTemplate *parseConstraint(std::string type, size_t boneA, size_t boneB);
	stream in;
};

#endif

