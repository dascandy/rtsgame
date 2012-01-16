#include "ModelDeserializerMs3d.h"
#include <exception>
#include "ResourceFile.h"
#include "Model.h"
#include <assert.h>

#pragma pack(push, 1)
	struct ms3dHeader
	{
		char id[10];
		int version;
	};
	struct ms3dVertex
	{
		unsigned char flags; //unused
		float vals[3];
		unsigned char boneId;
		char refCount; //unused
	};
	struct ms3dTriangle
	{
		short flags; //unused
		short vertices[3];
		float vertexNormals[3][3];
		float s[3];
		float t[3];
		unsigned char smoothingGroup;
		unsigned char groupId;
	};
	struct ms3dGroupHdr
	{
		char pad[33]; //unused
		unsigned short skipcount; //unused
	};
	struct ms3dMaterial
	{
		char _name[32];
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float emissive[4];
		float shininess;
		float transparency;
		char mode;
		char name[128];
		char alphamap[128]; //unused
	};
	struct ms3dBone
	{
		unsigned char flags;
		char name[32];
		char parentName[32];
		float rotation[3];
		float position[3];

		unsigned short numKeyFramesRot;
		unsigned short numKeyFramesTrans;
	};
#pragma pack(pop)

ModelDeserializerMs3d::ModelDeserializerMs3d(std::string filename)
: os(ResourceFile::open(filename, ResourceFile::Ms3dModel))
, nextSmoothingGroup(1)
{
}

Model *ModelDeserializerMs3d::deserialize()
{
	ms3dHeader hdr;
	unsigned short count;
	os.read((char *)&hdr, sizeof(hdr));

	if (hdr.version != 4) throw std::exception();

	os.read((char *)&count, sizeof(count));
	for (int i=0; i<count; i++)
	{
		deserializePoint3D();
	}

	os.read((char *)&count, sizeof(count));
	for (int i=0; i<count; i++)
	{
		deserializeTriangle();
	}

	Smooth();

	return new Model(vertices.size(), &vertices[0]);
}

void ModelDeserializerMs3d::deserializePoint3D()
{
	ms3dVertex data;
	os.read((char *)&data, sizeof(data));
	Model::vertex v = {0};
	v.x = data.vals[0];
	v.y = data.vals[1];
	v.z = data.vals[2];
	points.push_back(v);
}

void ModelDeserializerMs3d::deserializeTriangle()
{
	ms3dTriangle data;
	os.read((char *)&data, sizeof(data));

	for (int i = 0; i < 3; i++)
	{
		Model::vertex vertex = points[data.vertices[i]];
		vertex.nx = data.vertexNormals[i][0];
		vertex.ny = data.vertexNormals[i][1];
		vertex.nz = data.vertexNormals[i][2];
		vertex.tx1 = (unsigned short)(data.s[i]*32767);
		vertex.ty1 = (unsigned short)(data.t[i]*32767);
		if (smoothingGroups[vertex] == 0)
		{
			smoothingGroups[vertex] = nextSmoothingGroup;
		}
		vertex.smoothId = smoothingGroups[vertex];
		vertices.push_back(vertex); 
	}
	nextSmoothingGroup++;
}

void ModelDeserializerMs3d::Smooth()
{
	bool someChange;
	do
	{
		someChange = false;
		for (size_t i = 0; i < vertices.size(); i+=3)
		{
			unsigned short idx1 = smoothingGroups[vertices[i]];
			unsigned short idx2 = smoothingGroups[vertices[i+1]];
			unsigned short idx3 = smoothingGroups[vertices[i+2]];
			unsigned short minIndex = min(idx1, min(idx2, idx3));
			unsigned short maxIndex = max(idx1, max(idx2, idx3));
			if (minIndex != maxIndex)
			{
				someChange = true;
				vertices[i].smoothId = minIndex;
				vertices[i+1].smoothId = minIndex;
				vertices[i+2].smoothId = minIndex;
				smoothingGroups[vertices[i]] = minIndex;
				smoothingGroups[vertices[i+1]] = minIndex;
				smoothingGroups[vertices[i+2]] = minIndex;
			}
		}
	}
	while (someChange);
}

