#include "GameModel.h"
#include "Model.h"
#include "Texture.h"

#define LINE_LIMIT 1024
Res<Texture> GameModel::models;
Res<Texture> GameModel::modelxyzs;
Res<Texture> GameModel::modelnormt;
Res<Texture> GameModel::modelargs;
int GameModel::lastLine = 0;
unsigned short GameModel::lastId = 0;

void GameModel::Construct() {
	models = Res<Texture>(new Texture(256, 1, GL_RGBA32I));
	modelxyzs = Res<Texture>(new Texture(256, LINE_LIMIT, GL_RGBA32F));
	modelnormt = Res<Texture>(new Texture(256, LINE_LIMIT, GL_RGBA32F));
	modelargs = Res<Texture>(new Texture(256, LINE_LIMIT, GL_RGBA8));
}

GameModel::GameModel(int vertexcount, Model::vertex *vertices) {
	if (!models.isLoaded()) {
		Construct();
	}

	int linecount = (vertexcount + 255) / 256;
	if (lastLine + linecount > LINE_LIMIT) TODO("Allocate more space");
	if (lastId == 255) TODO("Allocate more space");
	id = lastId++;
	int start = lastLine;
	lastLine += linecount;
	int vals[4] = { start, linecount, vertexcount % 256, 0 };
	models->SetSubimage(id, 0, 1, 1, vals);

	float xyzs[1024];
	float normt[1024];
	unsigned char args[1024];
	for (int i = 0; i < linecount; i++) {
		int j = 0;
		for (; j < 256; j++) {
			if (i*256+j == vertexcount) break;
			xyzs[j*4+0] = vertices[i*256+j].x;
			xyzs[j*4+1] = vertices[i*256+j].y;
			xyzs[j*4+2] = vertices[i*256+j].z;
			xyzs[j*4+3] = vertices[i*256+j].s0;
			normt[j*4+0] = vertices[i*256+j].nx;
			normt[j*4+1] = vertices[i*256+j].ny;
			normt[j*4+2] = vertices[i*256+j].nz;
			normt[j*4+3] = vertices[i*256+j].t0;
			args[j*4+0] = vertices[i*256+j].boneId;
			args[j*4+1] = vertices[i*256+j].materialType;
			args[j*4+2] = vertices[i*256+j].smoothId;
			args[j*4+3] = vertices[i*256+j].pad;
		}

		modelxyzs->SetSubimage(0, start + i, j, 1, xyzs);
		modelnormt->SetSubimage(0, start + i, j, 1, normt);
		modelargs->SetSubimage(0, start + i, j, 1, args);
	}
}

