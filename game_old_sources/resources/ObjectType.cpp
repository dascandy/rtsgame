#include "ObjectType.h"
#include "PointLight.h"
#include "Vector3.h"
#include "Color.h"
#include "debug.h"
#include "stringutil.h"
#include "Texture.h"
#include "TextureDeserializer.h"
#include "Object.h"
#include "BehaviourType.h"

Texture *ObjectType::dSpecularT = 0;
Texture *ObjectType::dEmissiveT = 0;
Texture *ObjectType::dNormalT = 0;

ObjectType::ObjectType(Model *model, PhysicsModel *physicsModel, Texture *texture, BehaviourType *behaviour, const std::map<std::string, std::string> &args, float mass, float masssd)
: gfxModel(model)
, physicsModel(physicsModel)
, texture(texture)
, specularT(dSpecularT)
, emissiveT(dEmissiveT)
, normalT(dNormalT)
, behaviour(behaviour)
, mass(mass)
, masssd(masssd)
, args(args)
{
	if (!dSpecularT || !dNormalT || !dEmissiveT)
	{
		dSpecularT = specularT = TextureDeserializer("blank_SPEC").deserialize();
		dNormalT = normalT = TextureDeserializer("blank_NRM").deserialize();
		dEmissiveT = emissiveT = TextureDeserializer("blank_EMI").deserialize();
	}
	for (std::map<std::string, std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
	{
		if (it->first.substr(0, 8) == "specular")
		{
			try
			{
				specularT = TextureDeserializer(it->second).deserialize();
			} 
			catch(std::exception &e)
			{
				printf("Error loading specular: %s\n", e.what());
			}
			if (!specularT)
				specularT = dSpecularT;
		}
		else if (it->first.substr(0, 8) == "emissive")
		{
			try
			{
				emissiveT = TextureDeserializer(it->second).deserialize();
			} 
			catch(std::exception e)
			{
				printf("Error loading emissive: %s\n", e.what());
			}
			if (!emissiveT)
				emissiveT = dEmissiveT;
		}
		else if (it->first.substr(0, 6) == "normal")
		{
			try
			{
				normalT = TextureDeserializer(it->second).deserialize();
			} 
			catch(std::exception e)
			{
				printf("Error loading normal map: %s\n", e.what());
			}
			if (!normalT)
				normalT = dNormalT;
		}
		else if (it->first.substr(0, 4) == "name")
		{
			name = it->second;
		}
		else if (it->first.substr(0, 10) == "pointlight")
		{
			std::vector<std::string> substrs = split(it->second);
			if (substrs.size() != 6)
				ERR("Insufficient arguments");
			else
			{
				float r = (float)atof(substrs[0].c_str()),
					  g = (float)atof(substrs[1].c_str()),
					  b = (float)atof(substrs[2].c_str()),
					  x = (float)atof(substrs[3].c_str()),
					  y = (float)atof(substrs[4].c_str()),
					  z = (float)atof(substrs[5].c_str());

				pointLights.push_back(PointLight(Color(r, g, b), Vector3(x, y, z)));
			}
		}
		else if (it->first.substr(0, 9) == "spotlight")
		{
			std::vector<std::string> substrs = split(it->second);
			if (substrs.size() != 12)
				ERR("Insufficient arguments");
			else
			{
				float r = (float)atof(substrs[0].c_str()),
					  g = (float)atof(substrs[1].c_str()),
					  b = (float)atof(substrs[2].c_str()),
					  x = (float)atof(substrs[3].c_str()),
					  y = (float)atof(substrs[4].c_str()),
					  z = (float)atof(substrs[5].c_str()),
					  w_ = (float)atof(substrs[6].c_str()),
					  x_ = (float)atof(substrs[7].c_str()),
					  y_ = (float)atof(substrs[8].c_str()),
					  z_ = (float)atof(substrs[9].c_str()),
					  a = (float)atof(substrs[10].c_str());

				spotLights.push_back(SpotLight(Color(r, g, b), Vector3(x, y, z), Quaternion(w_, x_, y_, z_), a, TextureDeserializer(substrs[11]).deserialize()));
			}
		}
	}
}

Object *ObjectType::Create(const std::vector<Vector3> &translation, const std::vector<Quaternion> &rotation)
{
	Object *obj = new Object(*this, translation, rotation);
	obj->behaviour = this->behaviour->Create(obj, args);
	return obj;
}

Object *ObjectType::Create(const Vector3 &translation, const Quaternion &rotation)
{
	Object *obj = new Object(*this, translation, rotation);
	obj->behaviour = this->behaviour->Create(obj, args);
	return obj;
}

