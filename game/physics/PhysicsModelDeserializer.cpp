#include <SDL.h>
#include "stream.h"
#include "ResourceFile.h"
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <vector>
#include "PhysicsModelDeserializer.h"
#include "Quaternion.h"
#include "stringutil.h"
#include "debug.h"
#include "BoneTemplate.h"
#include "P2PTemplate.h"

PhysicsModelDeserializer::PhysicsModelDeserializer(std::string name)
: in(ResourceFile::open(name, ResourceFile::PhysicsModel))
{
}

PhysicsModel *PhysicsModelDeserializer::deserialize()
{
	PhysicsModel *pm = new PhysicsModel();
	while(!in.eof())
	{
		std::string line = in.readline();
		std::vector<std::string> items = split(line);
		if (items.size() == 0) continue;

		if (items[0] == "bone")
		{
			float mass = 0;
			float masssd = 0;
			float x = 0, y = 0, z = 0;
			for (std::vector<std::string>::iterator it = ++items.begin(); it != items.end(); ++it)
			{
				std::vector<std::string> mapping = split(*it, '=');
				if (mapping[0] == "id")
				{
					// ignore
				}
				else if (mapping[0] == "mass")
				{
					mass = (float)atof(mapping[1].c_str());
				}
				else if (mapping[0] == "mass_sd")
				{
					masssd = (float)atof(mapping[1].c_str());
				}
				else if (mapping[0] == "location")
				{
					std::vector<std::string> points = split(mapping[1], '/');
					if (points.size() != 3) continue;
					x = (float)atof(points[0].c_str());
					y = (float)atof(points[1].c_str());
					z = (float)atof(points[2].c_str());
				}
				else
				{
					TODO("Unknown attribute %s", mapping[0].c_str());
				}
			}
			btCollisionShape *shape = deserializeShape();
			BoneTemplate *bt = new BoneTemplate(Vector3(x, y, z), Quaternion(), shape, mass, masssd);
			pm->AddBoneTemplate(bt);
		}
		else if (items[0] == "constraint")
		{
			size_t boneA = atoi(items[2].c_str());
			size_t boneB = atoi(items[3].c_str());
			ConstraintTemplate *ct = parseConstraint(items[1], boneA, boneB);
			pm->AddConstraintTemplate(ct);
		}
	}
	if (pm->templates.size() == 0)
	{
		pm->AddBoneTemplate(new BoneTemplate(Vector3(), Quaternion(), NULL, 0, 0));
	}
	return pm;
}

ConstraintTemplate *PhysicsModelDeserializer::parseConstraint(std::string type, size_t boneA, size_t boneB)
{
	if (type == "p2p")
	{
		std::vector<std::string> items = split(in.readline());
		float x1 = (float)atof(items[0].c_str());
		float y1 = (float)atof(items[1].c_str());
		float z1 = (float)atof(items[2].c_str());
		float x2 = (float)atof(items[3].c_str());
		float y2 = (float)atof(items[4].c_str());
		float z2 = (float)atof(items[5].c_str());
		return new P2PTemplate(boneA, boneB, Vector3(x1, y1, z1), Vector3(x2, y2, z2));
	}
	else
	{
		TODO("Create constraint type %s", type.c_str());
		return NULL;
	}
}

btCollisionShape *PhysicsModelDeserializer::deserializeShape() 
{
	btCompoundShape *model;
	std::string line = in.readline();
	std::vector<std::string> items = split("0 0 0 0 0 0 " + line);
	if (items.size() < 7) return NULL;
	if (items[6] == "compound")
	{
		model = new btCompoundShape();
		while (!in.eof())
		{
			line = in.readline();
			std::vector<std::string> items = split(line);
			if (items.size() < 6) return model;

			btCollisionShape *shape = parseSimpleShape(items);
			float x = (float)atof(items[0].c_str()), 
				  y = (float)atof(items[1].c_str()), 
				  z = (float)atof(items[2].c_str()),
				  yaw = (float)atof(items[3].c_str()), 
				  pitch = (float)atof(items[4].c_str()), 
				  roll = (float)atof(items[5].c_str());
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

btCollisionShape *PhysicsModelDeserializer::parseSimpleShape(std::vector<std::string> &items)
{
	btCollisionShape *shape = NULL;
	if (items.size() < 7) return shape;
	if (items[6] == "box" && items.size() == 10)
	{
		float x = (float)atof(items[7].c_str()), 
			  y = (float)atof(items[8].c_str()), 
			  z = (float)atof(items[9].c_str());
		shape = new btBoxShape(btVector3(x/2,y/2,z/2));
	}
	else if (items[6] == "cylinder" && items.size() == 9)
	{
		float h = (float)atof(items[7].c_str()), 
			  d = (float)atof(items[8].c_str());
		shape =  new btCylinderShape(btVector3(d/2,h/2,d/2));
	}
	else if (items[6] == "sphere" && items.size() == 8)
	{
		float d = (float)atof(items[7].c_str());
		shape =  new btSphereShape(d/2);
	}
	else 
	{
		LOG_WARN("Incorrect argument count %d to type %s", items.size(), items[6].c_str());
	}
	return shape;
}

