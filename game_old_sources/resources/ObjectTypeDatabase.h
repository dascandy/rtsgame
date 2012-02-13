#ifndef OBJECTTYPEDATABASE_H
#define OBJECTTYPEDATABASE_H

#include <string>
#include <map>
#include <vector>
#include "Physics.h"
#include "debug.h"
class ObjectType;
class Model;
class Texture;

class ObjectTypeDatabase
{
public:
	static ObjectTypeDatabase &Instance();
	ObjectTypeDatabase();
	~ObjectTypeDatabase();
	ObjectType &operator[](const std::string &index) 
	{
		if (objectTypes.find(index) == objectTypes.end())
			TODO("Missing object type %s", index.c_str());

		return *objectTypes[index]; 
	}
	std::map<std::string, std::map<std::string, ObjectType *> > categories;
private:
	Model *LoadModel(std::string name);
	void MakeObjectType(const std::string &category, std::map<std::string, std::string> &objargs);

	PhysicsModel *LoadPhysicsModel(std::string name);
	Texture *LoadTexture(std::string name);
	std::map<std::string, ObjectType *> objectTypes;
	std::map<std::string, Model *> models;
	std::map<std::string, PhysicsModel *> physicsModels;
	std::map<std::string, Texture *> textures;
};

#endif

