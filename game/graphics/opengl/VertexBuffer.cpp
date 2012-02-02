#include "VertexBuffer.h"
#include <stdio.h>
#include "swap.h"

int num_tris = 0;

void VertexBuffer::Construct(const VertexBuffer::reference *refs)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (int i = 0; refs[i].count; ++i)
	{
		glEnableVertexAttribArray(i);
		if (refs[i].type == GL_FLOAT)
			glVertexAttribPointer(i, refs[i].count, refs[i].type, GL_FALSE, stride, (void *)refs[i].offset);
		else
			glVertexAttribIPointer(i, refs[i].count, refs[i].type, stride, (void *)refs[i].offset);
	}
}

VertexBuffer::VertexBuffer(const reference *refs, size_t size, int stride, int type)
: count(0)
, type(type)
, stride(stride)
{
	Construct(refs);
	glBufferData(GL_ARRAY_BUFFER, stride*size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void VertexBuffer::Add(void *objs, size_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, this->count * stride, count * stride, objs);
	this->count += count;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Read(void *out)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, stride*count, out);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Write(void *in)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, stride*count, in);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Draw(ShaderProgram &)
{
	glBindVertexArray(vao);
	glDrawArrays(type, 0, count);
	glBindVertexArray(0);
	num_tris += count;
}

void swap(VertexBuffer &a, VertexBuffer &b)
{
	swap(a.vbo, b.vbo);
	swap(a.vao, b.vao);
	swap(a.count, b.count);
	swap(a.type, b.type);
	swap(a.stride, b.stride);
}

