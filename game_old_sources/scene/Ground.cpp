#include "Ground.h"
#include "Model.h"
#include "Physics.h"
#include "Texture.h"
#include "TextureDeserializer.h"
#include "debug.h"
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Quaternion.h"

Ground::Ground() 
{
	heights = TextureDeserializer("heightmap").deserialize();
	w = heights->w;
	h = heights->h;
	unsigned char *buffer = heights->read();
	ground = new float[w*h];
	for (int i = 0; i < w*h; i++) {
		ground[i] = (buffer[i*4+2] * 2 + buffer[i*4+1]/128.0f + buffer[i*4]/32768.0f) - 256.0f;
	}
	tex = TextureDeserializer("blank").deserialize();
	physicsHandle = IPhysics::Instance().AddGround(ground, w, h, 1);
	generateModel();
}

void Ground::Draw(ShaderProgram &prog) 
{
	prog.Set("mat_model[0]", Matrix());
	bool bound = tex->Bind(prog, "modeltex");
	bool boundS = tex->Bind(prog, "speculartex");
	bool boundN = tex->Bind(prog, "normaltex");
	bool boundE = tex->Bind(prog, "emissivetex");
	model->Draw(prog);
	if (bound) prog.curtex--;
	if (boundS) prog.curtex--;
	if (boundN) prog.curtex--;
	if (boundE) prog.curtex--;
}

void Ground::generateModel() {
	Model::vertex *vts = new Model::vertex[6*(w-1)*(h-1)];

	float xd = 1.0f/w;
	float yd = 1.0f/h;
	float nx = 0, ny = 0, nz = 1;
	float *hp = ground;
	for (int y = 0; y < h-1; y++) {
		for (int x = 0; x < w-1; x++) {
			vts[(y*(w-1)+x) * 6 + 0].x = (float)x - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 1].x = (float)x - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 2].x = (float)x + 1 - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 3].x = (float)x - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 4].x = (float)x + 1 - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 5].x = (float)x + 1 - w/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 0].z = (float)y - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 1].z = (float)y + 1 - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 2].z = (float)y - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 3].z = (float)y + 1 - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 4].z = (float)y + 1 - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 5].z = (float)y - h/2+0.5f;
			vts[(y*(w-1)+x) * 6 + 0].y = hp[0];
			vts[(y*(w-1)+x) * 6 + 1].y = hp[w];
			vts[(y*(w-1)+x) * 6 + 2].y = hp[1];
			vts[(y*(w-1)+x) * 6 + 3].y = hp[w];
			vts[(y*(w-1)+x) * 6 + 4].y = hp[w+1];
			vts[(y*(w-1)+x) * 6 + 5].y = hp[1];
			vts[(y*(w-1)+x) * 6 + 0].s0 = (float)x / w;
			vts[(y*(w-1)+x) * 6 + 1].s0 = (float)x / w;
			vts[(y*(w-1)+x) * 6 + 2].s0 = (float)x / w + xd;
			vts[(y*(w-1)+x) * 6 + 3].s0 = (float)x / w;
			vts[(y*(w-1)+x) * 6 + 4].s0 = (float)x / w + xd;
			vts[(y*(w-1)+x) * 6 + 5].s0 = (float)x / w + xd;
			vts[(y*(w-1)+x) * 6 + 0].t0 = (float)y / h;
			vts[(y*(w-1)+x) * 6 + 1].t0 = (float)y / h + yd;
			vts[(y*(w-1)+x) * 6 + 2].t0 = (float)y / h;
			vts[(y*(w-1)+x) * 6 + 3].t0 = (float)y / h + yd;
			vts[(y*(w-1)+x) * 6 + 4].t0 = (float)y / h + yd;
			vts[(y*(w-1)+x) * 6 + 5].t0 = (float)y / h;
			vts[(y*(w-1)+x) * 6 + 0].nx = nx;
			vts[(y*(w-1)+x) * 6 + 1].nx = nx;
			vts[(y*(w-1)+x) * 6 + 2].nx = nx;
			vts[(y*(w-1)+x) * 6 + 3].nx = nx;
			vts[(y*(w-1)+x) * 6 + 4].nx = nx;
			vts[(y*(w-1)+x) * 6 + 5].nx = nx;
			vts[(y*(w-1)+x) * 6 + 0].ny = ny;
			vts[(y*(w-1)+x) * 6 + 1].ny = ny;
			vts[(y*(w-1)+x) * 6 + 2].ny = ny;
			vts[(y*(w-1)+x) * 6 + 3].ny = ny;
			vts[(y*(w-1)+x) * 6 + 4].ny = ny;
			vts[(y*(w-1)+x) * 6 + 5].ny = ny;
			vts[(y*(w-1)+x) * 6 + 0].nz = nz;
			vts[(y*(w-1)+x) * 6 + 1].nz = nz;
			vts[(y*(w-1)+x) * 6 + 2].nz = nz;
			vts[(y*(w-1)+x) * 6 + 3].nz = nz;
			vts[(y*(w-1)+x) * 6 + 4].nz = nz;
			vts[(y*(w-1)+x) * 6 + 5].nz = nz;
			vts[(y*(w-1)+x) * 6 + 0].boneId = 0;
			vts[(y*(w-1)+x) * 6 + 1].boneId = 0;
			vts[(y*(w-1)+x) * 6 + 2].boneId = 0;
			vts[(y*(w-1)+x) * 6 + 3].boneId = 0;
			vts[(y*(w-1)+x) * 6 + 4].boneId = 0;
			vts[(y*(w-1)+x) * 6 + 5].boneId = 0;
			hp++;
		}		 
	}			 

	model = new Model(6*(w-1)*(h-1), vts);

	delete [] vts;
}

