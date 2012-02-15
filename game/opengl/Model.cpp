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
				

