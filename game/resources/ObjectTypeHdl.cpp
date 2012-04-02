#include <SDL/SDL.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <vector>
#include "debug.h"
#include "BoneTemplate.h"
#include "P2PTemplate.h"
#include "ResourceManager.h"
#include "Physics.h"

class ObjectTypeReader : public ResourceTypeHandler<ObjectType> {
public:
	ObjectTypeReader();
	ObjectType *load(Blob &b, const char *name);
	const char *getExt() { return "fnt"; }
private:
	ConstraintTemplate *parseConstraint(char *&line, char *type, size_t boneA, size_t boneB);
	btCollisionShape *deserializeShape(char *&line);
	btCollisionShape *parseSimpleShape(std::vector<char *> &items);
};

static ObjectTypeReader _readerFnt;

ObjectTypeReader::ObjectTypeReader() 
{
	ResourceManager::Instance().RegisterResourceTypeHandler<ObjectType>(*this);
}

static char *getLine(char *&linePtr) {
	if (linePtr == 0) return 0;
	char *line = linePtr;
	while (*linePtr != '\n' && *linePtr != 0) linePtr++;
	if (*linePtr == 0) {
		linePtr = 0;
		return line;
	}
	if (linePtr[-1] == '\r') linePtr[-1] = 0;
	*linePtr = 0;
	linePtr++;
	return line;
}

std::vector<char *> split(char *line, char separator = ' ') {
	std::vector<char *> items;
	items.push_back(line);
	for (;;) {
		while (*line != 0 && *line != separator) ++line;
		if (*line == 0) break;
		*line = 0;
		++line;
		while (*line == separator) ++line;
		if (*line == 0) break;
		items.push_back(line);
	}
	return items;
}

ObjectType *ObjectTypeReader::load(Blob &b, const char *name)
{
	Blob b2(b.size);
	memcpy(b2.data, b.data, b.size);
	char *buf = b2.data;
	ObjectType *pm = new ObjectType();
	for (char *line = getLine(buf); line; line = getLine(buf))
	{
		std::vector<char *> items = split(line);
		if (items.size() == 0) continue;

		if (strcmp(items[0], "bone") == 0)
		{
			float mass = 0;
			float x = 0, y = 0, z = 0;
			for (std::vector<char *>::iterator it = ++items.begin(); it != items.end(); ++it)
			{
				std::vector<char *> mapping = split(*it, '=');
				if (strcmp(mapping[0], "id") == 0)
				{
					// ignore
				}
				else if (strcmp(mapping[0], "mass") == 0)
				{
					mass = (float)atof(mapping[1]);
				}
				else if (strcmp(mapping[0], "location") == 0)
				{
					std::vector<char *> points = split(mapping[1], '/');
					if (points.size() != 3) continue;
					x = (float)atof(points[0]);
					y = (float)atof(points[1]);
					z = (float)atof(points[2]);
				}
				else
				{
					TODO("Unknown attribute %s", mapping[0]);
				}
			}
			btCollisionShape *shape = deserializeShape(line);
			BoneTemplate *bt = new BoneTemplate(vec3(x, y, z), quat(), shape, mass);
			pm->AddBoneTemplate(bt);
		}
		else if (strcmp(items[0], "constraint") == 0)
		{
			size_t boneA = atoi(items[2]);
			size_t boneB = atoi(items[3]);
			ConstraintTemplate *ct = parseConstraint(line, items[1], boneA, boneB);
			pm->AddConstraintTemplate(ct);
		}
	}
	if (pm->templates.size() == 0)
	{
		pm->AddBoneTemplate(new BoneTemplate(vec3(), quat(), NULL, 0));
	}
	return pm;
}

ConstraintTemplate *ObjectTypeReader::parseConstraint(char *&line, char *type, size_t boneA, size_t boneB)
{
	if (strcmp(type, "p2p") == 0)
	{
		std::vector<char *> items = split(getLine(line));
		float x1 = (float)atof(items[0]);
		float y1 = (float)atof(items[1]);
		float z1 = (float)atof(items[2]);
		float x2 = (float)atof(items[3]);
		float y2 = (float)atof(items[4]);
		float z2 = (float)atof(items[5]);
		return new P2PTemplate(boneA, boneB, vec3(x1, y1, z1), vec3(x2, y2, z2));
	}
	else
	{
		TODO("Create constraint type %s", type);
		return NULL;
	}
}

btCollisionShape *ObjectTypeReader::deserializeShape(char *&line) 
{
	btCompoundShape *model;
	char *curLine = getLine(line);
	char *cline = (char *)malloc(13 + strlen(curLine));
	sprintf(cline, "0 0 0 0 0 0 %s", curLine);
	std::vector<char *> items = split(cline);
	if (items.size() < 7) return NULL;
	if (strcmp(items[6], "compound") == 0)
	{
		model = new btCompoundShape();
		while (line != 0)
		{
			curLine = getLine(line);
			std::vector<char *> items = split(curLine);
			if (items.size() < 6) return model;

			btCollisionShape *shape = parseSimpleShape(items);
			float x = (float)atof(items[0]), 
				  y = (float)atof(items[1]), 
				  z = (float)atof(items[2]),
				  yaw = (float)atof(items[3]), 
				  pitch = (float)atof(items[4]), 
				  roll = (float)atof(items[5]);
			model->addChildShape(btTransform(btQuaternion(yaw / 57.17f, pitch / 57.17f, roll / 57.17f), btVector3(x, y, z)), shape);
		}
		return model;
	}
	else
	{
		btCollisionShape *shape = parseSimpleShape(items);
		return shape;
	}
}

btCollisionShape *ObjectTypeReader::parseSimpleShape(std::vector<char *> &items)
{
	btCollisionShape *shape = NULL;
	if (items.size() < 7) return shape;
	if (strcmp(items[6], "box") == 0 && items.size() == 10)
	{
		float x = (float)atof(items[7]), 
			  y = (float)atof(items[8]), 
			  z = (float)atof(items[9]);
		shape = new btBoxShape(btVector3(x/2,y/2,z/2));
	}
	else if (strcmp(items[6], "cylinder") == 0 && items.size() == 9)
	{
		float h = (float)atof(items[7]), 
			  d = (float)atof(items[8]);
		shape =  new btCylinderShape(btVector3(d/2,h/2,d/2));
	}
	else if (strcmp(items[6], "sphere") == 0 && items.size() == 8)
	{
		float d = (float)atof(items[7]);
		shape =  new btSphereShape(d/2);
	}
	else 
	{
		LOG_WARN("Incorrect argument count %d to type %s", items.size(), items[6]);
	}
	return shape;
}

