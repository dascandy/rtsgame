#include "ObjectTypeDatabase.h"
#include "PhysicsModelDeserializer.h"
#include "TextureDeserializer.h"
#include "ResourceFile.h"
#include "stream.h"
#include "ObjectType.h"
#include "BehaviourType.h"
#include "debug.h"
#include "Model.h"
#include "BoneTemplate.h"
#include "profile.h"

ObjectTypeDatabase &ObjectTypeDatabase::Instance()
{
	static ObjectTypeDatabase database;
	return database;
}

ObjectTypeDatabase::ObjectTypeDatabase()
{
	std::string category;
	std::map<std::string, std::string> objargs;
	enum { None, Category, Object } mode = None;

	stream in = ResourceFile::open("objects", ResourceFile::DatFile);
	while (!in.eof())
	{
		std::string line = in.readline();
		if (line[0] == '[')
		{
			if (mode == Object)
			{
				MakeObjectType(category, objargs);
			}

			if (line == "[category]")
				mode = Category;
			else if (line == "[object]")
				mode = Object;
			else
				mode = None;
		}
		else if (line.find_first_of('=') != std::string::npos)
		{
			std::string beforeEq = line.substr(0, line.find_first_of('=')), 
						afterEq = line.substr(line.find_first_of('=')+1);

			switch(mode)
			{
			case Category:
				if (beforeEq == "name")
				{
					category = afterEq;
				}
				else
					LOG_WARN("Unknown attribute for category: %s => %s", beforeEq.c_str(), afterEq.c_str());
				break;
			case Object:
				objargs.insert(std::make_pair(beforeEq, afterEq));
				break;
			}
		}
	}

	if (mode == Object)
	{
		MakeObjectType(category, objargs);
	}
}

void ObjectTypeDatabase::MakeObjectType(const std::string &category, std::map<std::string, std::string> &objargs)
{
	if ((objargs.find("name") != objargs.end()) &&
		(objargs.find("model") != objargs.end()))
	{
		Model *m = LoadModel(objargs["model"]);
		PhysicsModel *pm = LoadPhysicsModel(objargs["model"]);
		objargs.erase(objargs.find("model"));
		m->PreTranslate(pm);

		if (m == 0) 
			TODO("Create model");

		TODO_W("Make textures that are required");
		Texture *t = NULL;
		if (objargs.find("texture") != objargs.end())
		{
			t = LoadTexture(objargs["texture"]);
			objargs.erase(objargs.find("texture"));
		}

		BehaviourType *b;
		if (objargs.find("behaviour") != objargs.end() &&
			BehaviourType::List().find(objargs["behaviour"]) != BehaviourType::List().end())
			b = BehaviourType::List()[objargs["behaviour"]];
		else
			b = BehaviourType::List()["None"];

		if (objargs.find("behaviour") != objargs.end())
			objargs.erase(objargs.find("behaviour"));

		float mass = 0.0f;
		float masssd = 0.0f;
		if (objargs.find("mass") != objargs.end())
		{
			mass = (float)atof(objargs["mass"].c_str());
			objargs.erase(objargs.find("mass"));
			if (objargs.find("masssd") != objargs.end())
			{
				masssd = (float)atof(objargs["masssd"].c_str());
				objargs.erase(objargs.find("masssd"));
			}
		}

		std::string objname = objargs["name"];

		ObjectType *ot = new ObjectType(m, pm, t, b, objargs, mass, masssd);

		categories[category].insert(std::make_pair(objname, ot));
		objectTypes.insert(std::make_pair(objname, ot));
	}
	else
	{
		LOG_WARN("Invalid model found! Missing name or model.");
	}
	objargs.clear();
}

ObjectTypeDatabase::~ObjectTypeDatabase()
{

}

Model *ObjectTypeDatabase::LoadModel(std::string name)
{
	std::map<std::string, Model *>::iterator it = models.find(name);
	if (it != models.end())
		return (it->second);

	Model *model = &Model::Load(name);
	models.insert(std::make_pair(name, model));
	return model;
}

PhysicsModel *ObjectTypeDatabase::LoadPhysicsModel(std::string name)
{
	std::map<std::string, PhysicsModel *>::iterator it = physicsModels.find(name);
	if (it != physicsModels.end())
		return (it->second);

	PhysicsModel *physicsModel;
	try
	{
		physicsModel = PhysicsModelDeserializer(name).deserialize();
	}
	catch(std::exception &e) 
	{
		LOG_WARN("Couldn't load physics model %s because:%s", name.c_str(), e.what());
		physicsModel = new PhysicsModel();
		physicsModel->AddBoneTemplate(new BoneTemplate(Vector3(), Quaternion(), NULL, 0, 0));
	}

	physicsModels.insert(std::make_pair(name, physicsModel));
	return physicsModel;
}

Texture *ObjectTypeDatabase::LoadTexture(std::string name)
{
	std::map<std::string, Texture *>::iterator it = textures.find(name);
	if (it != textures.end())
		return (it->second);

	Texture *texture;
	try
	{
		texture = TextureDeserializer(name).deserialize();
	}
	catch(std::exception &e) 
	{
		LOG_WARN("Couldn't load texture %s because:%s", name.c_str(), e.what());
		texture = NULL; 
	}

	textures.insert(std::make_pair(name, texture));
	return texture;
}

