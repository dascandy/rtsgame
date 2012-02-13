#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Renderable.h"
#include <GL/glew.h>

class VertexBuffer : public Renderable {
public:
	struct reference {
		int offset;
		int count;
		int type;
	};
	void Construct(const reference *refs);
	VertexBuffer(const reference *refs, size_t size, int stride, int type = GL_TRIANGLES);
	template <typename T>
	VertexBuffer(const reference *refs, size_t size, const T *objs, size_t count, int type = GL_TRIANGLES)
	: count(count)
	, type(type)
	, stride(sizeof(T))
	{
		Construct(refs);
		if (size == count)
		{
			glBufferData(GL_ARRAY_BUFFER, stride*count, objs, GL_STATIC_DRAW);
		}
		else
		{
			glBufferData(GL_ARRAY_BUFFER, stride*size, 0, GL_DYNAMIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, stride*count, objs);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	~VertexBuffer();
	void Add(void *objs, size_t count);
	void Draw(ShaderProgram &);
	void Read(void *out);
	void Write(void *in);
	unsigned int vbo, vao;
	unsigned int count;
	unsigned int type;
	unsigned int stride;
};

void swap(VertexBuffer &a, VertexBuffer &b);

#endif

