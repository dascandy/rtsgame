#include "Model.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string.h>
#include <vector>
#include <map>
#include <utility>
//#include "PhysicsModel.h"

VertexBuffer::reference refs[] = 
{
	0, 3, GL_FLOAT,
	12, 3, GL_FLOAT,
	24, 2, GL_UNSIGNED_SHORT,
	28, 4, GL_UNSIGNED_BYTE,
	0, 0, 0
};

Model::Model(int vertexcount, vertex *vertices)
: vb(refs, vertexcount, vertices, vertexcount)
{
}

void Model::Draw(Res<ShaderProgram> &prog)
{
	vb.Draw(prog);
}
/*
void Model::PreTranslate(PhysicsModel *m)
{
	vertex *data = new vertex[vb.count];
	vb.Read(data);
	glm::mat4 *matrices = new glm::mat4[m->templates.size()];
	for (size_t i = 0; i < m->templates.size(); i++)
	{
		matrices[i] = glm::gtc::affineInverse(glm::translate(glm::rotate(glm::mat4(1.0f), m->templates[i]->rotation), m->templates[i]->translation));
	}
	for (size_t i = 0; i < vb.count; i++) 
	{
		glm::vec3 vertexloc(data[i].x, data[i].y, data[i].z);
		glm::vec3 transLoc = matrices[data[i].boneId] * vertexloc;
		data[i].x = transLoc.x;
		data[i].y = transLoc.y;
		data[i].z = transLoc.z;
	}
	vb.Write(data);
	delete [] matrices;
	delete [] data;
}
*/
Model &Model::Square()
{
	static vertex v[6] = {
		{ -1, -1, -0.5, 0, 0, 1,     0,     0, 0, 0 },
		{  1, -1, -0.5, 0, 0, 1, 65535,     0, 0, 0 },
		{  1,  1, -0.5, 0, 0, 1, 65535, 65535, 0, 0 },
		{  1,  1, -0.5, 0, 0, 1, 65535, 65535, 0, 0 },
		{ -1,  1, -0.5, 0, 0, 1,     0, 65535, 0, 0 },
		{ -1, -1, -0.5, 0, 0, 1,     0,     0, 0, 0 },
	};
	static Model square(6, v);
	return square;
}

Model &Model::TesselatedSquare(int tess) {
	vertex *vs = new vertex[tess * tess * 6];
	vertex *cv = vs;
	float dx = 2.0f / tess;
	float dy = 2.0f / tess;
	for (int i = 0; i < tess; i++) {
		for (int j = 0; j < tess; j++) {
			(*cv).x = -1 + i * dx;
			(*cv).y = -1 + j * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * i) * 65535 / 2.0f;
			(*cv).t0 = (dy * j) * 65535 / 2.0f;
			cv++;
			(*cv).x = -1 + (i+1) * dx;
			(*cv).y = -1 + j * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * (i+1)) * 65535 / 2.0f;
			(*cv).t0 = (dy * j) * 65535 / 2.0f;
			cv++;
			(*cv).x = -1 + (i+1) * dx;
			(*cv).y = -1 + (j+1) * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * (i+1)) * 65535 / 2.0f;
			(*cv).t0 = (dy * (j+1)) * 65535 / 2.0f;
			cv++;
			(*cv).x = -1 + (i+1) * dx;
			(*cv).y = -1 + (j+1) * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * (i+1)) * 65535 / 2.0f;
			(*cv).t0 = (dy * (j+1)) * 65535 / 2.0f;
			cv++;
			(*cv).x = -1 + i * dx;
			(*cv).y = -1 + (j+1) * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * i) * 65535 / 2.0f;
			(*cv).t0 = (dy * (j+1)) * 65535 / 2.0f;
			cv++;
			(*cv).x = -1 + i * dx;
			(*cv).y = -1 + j * dy;
			(*cv).z = (*cv).nx = (*cv).ny = (*cv).nz = 0;
			(*cv).s0 = (dx * i) * 65535 / 2.0f;
			(*cv).t0 = (dy * j) * 65535 / 2.0f;
			cv++;
		}
	}
	//TODO make this shared somehow
	return *new Model(tess*tess*6, vs);
}

Model &Model::Cube()
{
	static vertex v[] = {
		{ -1,  1, -1, -1,  0,  0,      0,      0, 0, 0 },
		{ -1, -1, -1, -1,  0,  0,  65535,      0, 0, 0 },
		{ -1, -1,  1, -1,  0,  0,  65535,  65535, 0, 0 },
		{ -1, -1,  1, -1,  0,  0,  65535,  65535, 0, 0 },
		{ -1,  1,  1, -1,  0,  0,      0,  65535, 0, 0 },
		{ -1,  1, -1, -1,  0,  0,      0,      0, 0, 0 },
		{ -1,  1,  1,  0,  0,  1,      0,      0, 0, 0 },
		{ -1, -1,  1,  0,  0,  1,  65535,      0, 0, 0 },
		{  1, -1,  1,  0,  0,  1,  65535,  65535, 0, 0 },
		{  1, -1,  1,  0,  0,  1,  65535,  65535, 0, 0 },
		{  1,  1,  1,  0,  0,  1,      0,  65535, 0, 0 },
		{ -1,  1,  1,  0,  0,  1,      0,      0, 0, 0 },
		{  1, -1,  1,  1,  0,  0,      0,      0, 0, 0 },
		{  1, -1, -1,  1,  0,  0,  65535,      0, 0, 0 },
		{  1,  1, -1,  1,  0,  0,  65535,  65535, 0, 0 },
		{  1,  1, -1,  1,  0,  0,  65535,  65535, 0, 0 },
		{  1,  1,  1,  1,  0,  0,      0,  65535, 0, 0 },
		{  1, -1,  1,  1,  0,  0,      0,      0, 0, 0 },
		{  1,  1, -1,  0,  0, -1,      0,      0, 0, 0 },
		{  1, -1, -1,  0,  0, -1,      0,  65535, 0, 0 },
		{ -1, -1, -1,  0,  0, -1,  65535,  65535, 0, 0 },
		{ -1, -1, -1,  0,  0, -1,  65535,  65535, 0, 0 },
		{ -1,  1, -1,  0,  0, -1,  65535,      0, 0, 0 },
		{  1,  1, -1,  0,  0, -1,      0,      0, 0, 0 },
		{ -1,  1, -1,  0,  1,  0,      0,      0, 0, 0 },
		{ -1,  1,  1,  0,  1,  0,  65535,      0, 0, 0 },
		{  1,  1,  1,  0,  1,  0,  65535,  65535, 0, 0 },
		{  1,  1,  1,  0,  1,  0,  65535,  65535, 0, 0 },
		{  1,  1, -1,  0,  1,  0,      0,  65535, 0, 0 },
		{ -1,  1, -1,  0,  1,  0,      0,      0, 0, 0 },
		{  1, -1, -1,  0, -1,  0,      0,      0, 0, 0 },
		{  1, -1,  1,  0, -1,  0,  65535,      0, 0, 0 },
		{ -1, -1,  1,  0, -1,  0,  65535,  65535, 0, 0 },
		{ -1, -1,  1,  0, -1,  0,  65535,  65535, 0, 0 },
		{ -1, -1, -1,  0, -1,  0,      0,  65535, 0, 0 },
		{  1, -1, -1,  0, -1,  0,      0,      0, 0, 0 },
	};									
										
	static Model cube(sizeof(v)/sizeof(v[0]), v);
	return cube;						
}										
				

