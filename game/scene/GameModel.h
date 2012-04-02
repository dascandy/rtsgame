#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"

class GameModel {
	static void Construct();
	static Res<Texture> models;
	static Res<Texture> modelxyzs;
	static Res<Texture> modelnormt;
	static Res<Texture> modelargs;
	static int lastLine;
	static unsigned short lastId;
public:
	GameModel(int vertexcount, Model::vertex *vertices);
	unsigned short id;
	int start, count;
	int boneCount;
	struct vertex
	{
		float x, y, z;
		float nx, ny, nz;
		unsigned short s0, t0;
		unsigned char boneId, materialType, smoothId, pad;
	};
};

#endif

