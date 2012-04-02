#include "GameModel.h"
#include "ResourceManager.h"
#include <map>

class GameModelReader : public ResourceTypeHandler<GameModel> {
public:
	GameModelReader();
	GameModel *load(Blob &b, const char *name);
	const char *getExt() { return "ms3d"; }
};

static GameModelReader _readerFnt;

GameModelReader::GameModelReader() 
{
	ResourceManager::Instance().RegisterResourceTypeHandler<GameModel>(*this);
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

static GameModel::vertex deserializePoint3D(Blob &b, int &offset)
{
	ms3dVertex data;
	memcpy(&data, b.data + offset, sizeof(data)); offset += sizeof(data);
	Model::vertex v = {0};
	v.x = data.vals[0];
	v.y = data.vals[1];
	v.z = data.vals[2];

	return v;
}

static void deserializeTriangle(Blob &b, int &offset, std::vector<GameModel::vertex> &points, std::vector<GameModel::vertex> &vertices)
{
	ms3dTriangle data;
	memcpy(&data, b.data + offset, sizeof(data)); offset += sizeof(data);

	int idxs[3] = {0, 2, 1};
	for (int i = 0; i < 3; i++)
	{
		int idx = idxs[i];
		GameModel::vertex vertex = points[data.vertices[idx]];
		vertex.nx = data.vertexNormals[idx][0];
		vertex.ny = data.vertexNormals[idx][1];
		vertex.nz = data.vertexNormals[idx][2];
		vertex.s0 = (unsigned short)(data.s[idx] * 65535);
		vertex.t0 = (unsigned short)(data.t[idx] * 65535);
		vertices.push_back(vertex); 
	}
}

Model *GameModelReader::load(Blob &b, const char *name) {
	std::vector<GameModel::vertex> points;
	std::vector<GameModel::vertex> vertices;
	int offset = 0;

	ms3dHeader hdr;
	unsigned short count;
	memcpy(&hdr, b.data + offset, sizeof(hdr)); offset += sizeof(hdr);

	if (hdr.version != 4) throw std::exception();

	memcpy(&count, b.data + offset, sizeof(count)); offset += sizeof(count);
	for (int i=0; i<count; i++)
	{
		points.push_back(deserializePoint3D(b, offset));
	}

	memcpy(&count, b.data + offset, sizeof(count)); offset += sizeof(count);
	for (int i=0; i<count; i++)
	{
		deserializeTriangle(b, offset, points, vertices);
	}

	return new GameModel(vertices.size(), &vertices[0]);
}

