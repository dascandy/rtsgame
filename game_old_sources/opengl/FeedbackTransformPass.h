#ifndef FEEDBACKTRANSFORMPASS_H
#define FEEDBACKTRANSFORMPASS_H

#include "RenderPass.h"
#include "ShaderProgram.h"

class VertexBuffer;

class FeedbackTransformPass : public GpuRenderPass
{
public:
	FeedbackTransformPass(ShaderProgram program, VertexBuffer &output, VertexBuffer &buffer, int objtype);
	void DrawPick(std::map<Object *, int> &, std::vector<Object *> &) {}
	~FeedbackTransformPass();
private:
	void DoRun();
	VertexBuffer *out, *buf;
	unsigned int query;
	int objtype;
};

#endif

