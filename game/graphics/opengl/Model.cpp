#include "Model.h"
#include "ResourceFile.h"
#include "stream.h"
#include <gl/glew.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include "Physics.h"
#include "Matrix.h"
#include "BoneTemplate.h"
#include "Vector3.h"
#include "Quaternion.h"

VertexBuffer::reference refs[] = 
{
	0, 3, GL_FLOAT,
	12, 3, GL_FLOAT,
	24, 2, GL_FLOAT,
	32, 3, GL_UNSIGNED_BYTE,
	0, 0, 0
};

Model::Model(int vertexcount, vertex *vertices)
: vb(refs, vertexcount, vertices, vertexcount)
{
}

void Model::Draw(ShaderProgram &prog)
{
	vb.Draw(prog);
}

void Model::PreTranslate(PhysicsModel *m)
{
	vertex *data = new vertex[vb.count];
	vb.Read(data);
	Matrix *matrices = new Matrix[m->templates.size()];
	for (size_t i = 0; i < m->templates.size(); i++)
	{
		matrices[i] = Matrix(m->templates[i]->translation, m->templates[i]->rotation).Invert();
	}
	for (size_t i = 0; i < vb.count; i++) 
	{
		Vector3 vertexloc(data[i].x, data[i].y, data[i].z);
		Vector3 transLoc = matrices[data[i].boneId].transform(vertexloc);
		data[i].x = transLoc.x;
		data[i].y = transLoc.y;
		data[i].z = transLoc.z;
	}
	vb.Write(data);
	delete [] matrices;
	delete [] data;
}

Model &Model::Raster()
{
	static vertex v[12] = {
		{ -65536, 0, -65536, 0, 0, 1, -65536, -65536, 0, 0 },
		{ -65536, 0,  65536, 0, 0, 1, -65536,  65536, 0, 0 },
		{  65536, 0,  65536, 0, 0, 1,  65536,  65536, 0, 0 },
		{  65536, 0,  65536, 0, 0, 1,  65536,  65536, 0, 0 },
		{  65536, 0, -65536, 0, 0, 1,  65536, -65536, 0, 0 },
		{ -65536, 0, -65536, 0, 0, 1, -65536, -65536, 0, 0 },
		{ -65536, 0, -65536, 0, 0, 1, -65536, -65536, 0, 0 },
		{  65536, 0, -65536, 0, 0, 1,  65536, -65536, 0, 0 },
		{  65536, 0,  65536, 0, 0, 1,  65536,  65536, 0, 0 },
		{  65536, 0,  65536, 0, 0, 1,  65536,  65536, 0, 0 },
		{ -65536, 0,  65536, 0, 0, 1, -65536,  65536, 0, 0 },
		{ -65536, 0, -65536, 0, 0, 1, -65536, -65536, 0, 0 },
	};
	static Model square(12, v);
	return square;
}

Model &Model::Square()
{
	static vertex v[6] = {
		{ -1, -1, -0.5, 0, 0, 1, 0, 0, 0, 0 },
		{  1, -1, -0.5, 0, 0, 1, 1, 0, 0, 0 },
		{  1,  1, -0.5, 0, 0, 1, 1, 1, 0, 0 },
		{  1,  1, -0.5, 0, 0, 1, 1, 1, 0, 0 },
		{ -1,  1, -0.5, 0, 0, 1, 0, 1, 0, 0 },
		{ -1, -1, -0.5, 0, 0, 1, 0, 0, 0, 0 },
	};
	static Model square(6, v);
	return square;
}

Model &Model::Cube()
{
	static vertex v[] = {
		{ -1,  1, -1, -1,  0,  0,  0,  0, 0, 0 },
		{ -1, -1, -1, -1,  0,  0,  1,  0, 0, 0 },
		{ -1, -1,  1, -1,  0,  0,  1,  1, 0, 0 },
		{ -1, -1,  1, -1,  0,  0,  1,  1, 0, 0 },
		{ -1,  1,  1, -1,  0,  0,  0,  1, 0, 0 },
		{ -1,  1, -1, -1,  0,  0,  0,  0, 0, 0 },
		{ -1,  1,  1,  0,  0,  1,  0,  0, 0, 0 },
		{ -1, -1,  1,  0,  0,  1,  1,  0, 0, 0 },
		{  1, -1,  1,  0,  0,  1,  1,  1, 0, 0 },
		{  1, -1,  1,  0,  0,  1,  1,  1, 0, 0 },
		{  1,  1,  1,  0,  0,  1,  0,  1, 0, 0 },
		{ -1,  1,  1,  0,  0,  1,  0,  0, 0, 0 },
		{  1, -1,  1,  1,  0,  0,  0,  0, 0, 0 },
		{  1, -1, -1,  1,  0,  0,  1,  0, 0, 0 },
		{  1,  1, -1,  1,  0,  0,  1,  1, 0, 0 },
		{  1,  1, -1,  1,  0,  0,  1,  1, 0, 0 },
		{  1,  1,  1,  1,  0,  0,  0,  1, 0, 0 },
		{  1, -1,  1,  1,  0,  0,  0,  0, 0, 0 },
		{  1,  1, -1,  0,  0, -1,  0,  0, 0, 0 },
		{  1, -1, -1,  0,  0, -1,  0,  1, 0, 0 },
		{ -1, -1, -1,  0,  0, -1,  1,  1, 0, 0 },
		{ -1, -1, -1,  0,  0, -1,  1,  1, 0, 0 },
		{ -1,  1, -1,  0,  0, -1,  1,  0, 0, 0 },
		{  1,  1, -1,  0,  0, -1,  0,  0, 0, 0 },
		{ -1,  1, -1,  0,  1,  0,  0,  0, 0, 0 },
		{ -1,  1,  1,  0,  1,  0,  1,  0, 0, 0 },
		{  1,  1,  1,  0,  1,  0,  1,  1, 0, 0 },
		{  1,  1,  1,  0,  1,  0,  1,  1, 0, 0 },
		{  1,  1, -1,  0,  1,  0,  0,  1, 0, 0 },
		{ -1,  1, -1,  0,  1,  0,  0,  0, 0, 0 },
		{  1, -1, -1,  0, -1,  0,  0,  0, 0, 0 },
		{  1, -1,  1,  0, -1,  0,  1,  0, 0, 0 },
		{ -1, -1,  1,  0, -1,  0,  1,  1, 0, 0 },
		{ -1, -1,  1,  0, -1,  0,  1,  1, 0, 0 },
		{ -1, -1, -1,  0, -1,  0,  0,  1, 0, 0 },
		{  1, -1, -1,  0, -1,  0,  0,  0, 0, 0 },
	};									
										
	static Model cube(sizeof(v)/sizeof(v[0]), v);
	return cube;						
}										
										
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

Model::vertex deserializePoint3D(stream &file)
{
	ms3dVertex data;
	file.read((char *)&data, sizeof(data));
	Model::vertex v = {0};
	v.x = data.vals[0];
	v.y = data.vals[1];
	v.z = data.vals[2];
	v.boneId = data.boneId;
	if (v.boneId == 255) v.boneId = 0;

	return v;
}

void deserializeTriangle(stream &file, std::vector<Model::vertex> &points, std::vector<Model::vertex> &vertices, std::map<Model::vertex, unsigned short> &smoothingGroups, unsigned short &nextSmoothingGroup)
{
	ms3dTriangle data;
	file.read((char *)&data, sizeof(data));

	for (int i = 0; i < 3; i++)
	{
		Model::vertex vertex = points[data.vertices[i]];
		vertex.nx = data.vertexNormals[i][0];
		vertex.ny = data.vertexNormals[i][1];
		vertex.nz = data.vertexNormals[i][2];
		vertex.s0 = data.s[i];
		vertex.t0 = data.t[i];
		if (smoothingGroups[vertex] == 0)
		{
			smoothingGroups[vertex] = nextSmoothingGroup;
		}
		vertex.smoothId = (unsigned char)smoothingGroups[vertex];
		vertices.push_back(vertex); 
	}
	nextSmoothingGroup++;
}

void Smooth(std::vector<Model::vertex> &vertices, std::map<Model::vertex, unsigned short> &smoothingGroups)
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
			unsigned short minIndex = std::min(idx1, std::min(idx2, idx3));
			unsigned short maxIndex = std::max(idx1, std::max(idx2, idx3));
			if (minIndex != maxIndex)
			{
				someChange = true;
				vertices[i].smoothId = (unsigned char)minIndex;
				vertices[i+1].smoothId = (unsigned char)minIndex;
				vertices[i+2].smoothId = (unsigned char)minIndex;
				smoothingGroups[vertices[i]] = minIndex;
				smoothingGroups[vertices[i+1]] = minIndex;
				smoothingGroups[vertices[i+2]] = minIndex;
			}
		}
	}
	while (someChange);
}

Model &Model::Load(const std::string &name)
{
	try {
		return loadFromStream(ResourceFile::Instance().open(name, ResourceFile::Ms3dModel));
	} catch (std::exception &) {
		return loadFromStream(ResourceFile::Instance().open("OGA_" + name, ResourceFile::Ms3dModel));
	}
}

Model &Model::loadFromStream(stream file)
{
	std::vector<Model::vertex> points;
	std::vector<Model::vertex> vertices;
	std::map<Model::vertex, unsigned short> smoothingGroups;
	unsigned short nextSmoothingGroup = 1;

	ms3dHeader hdr;
	unsigned short count;
	file.read((char *)&hdr, sizeof(hdr));

	if (hdr.version != 4) throw std::exception();

	file.read((char *)&count, sizeof(count));
	for (int i=0; i<count; i++)
	{
		points.push_back(deserializePoint3D(file));
	}

	file.read((char *)&count, sizeof(count));
	for (int i=0; i<count; i++)
	{
		deserializeTriangle(file, points, vertices, smoothingGroups, nextSmoothingGroup);
	}

	Smooth(vertices, smoothingGroups);

	return *new Model(vertices.size(), &vertices[0]);
}

