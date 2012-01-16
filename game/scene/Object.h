#ifndef OBJECT_H
#define OBJECT_H

#include "Renderable.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <vector>
#include "Physics.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Behaviour.h"
class IFrustum;
class Light;
class ObjectType;
class ShaderProgram;

class Object : public Renderable
{
private:
	friend class ObjectType;
	Object(ObjectType &objectType, const std::vector<Vector3> &location, const std::vector<Quaternion> &rot);
	Object(ObjectType &objectType, const Vector3 &location, const Quaternion &rot);
public:
	Object(const Object &orig);
	~Object();
	void Draw(ShaderProgram &vs);
#ifndef NO_EDITING
	void setSelected(bool selected);
	bool isSelected();
#endif
	bool hasLights();
	void GetLights(std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights);
	PhysicsObject *physicsHandle;
#ifndef NO_EDITING
	bool selected;
#endif
protected:
	ObjectType &objectType;
public:
	Behaviour *behaviour;
	bool isStatic;
	std::vector<Bone *> bones;
	std::vector<PointLight *> pointLights;
	std::vector<SpotLight *> spotLights;
private:
	Object &operator=(const Object &);
	friend class MotionState;
	friend class SectorSerializer;
};

#endif

