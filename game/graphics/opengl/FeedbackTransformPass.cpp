#include "FeedbackTransformPass.h"
#include "VertexBuffer.h"
#include "profile.h"
#include "debug.h"

FeedbackTransformPass::FeedbackTransformPass(ShaderProgram program, VertexBuffer &output, VertexBuffer &buffer, int objtype)
: GpuRenderPass(program)
, out(&output)
, buf(&buffer)
, objtype(objtype)
{
	glGenQueries(1, &query);
}

FeedbackTransformPass::~FeedbackTransformPass()
{
	glDeleteQueries(1, &query);
}

void FeedbackTransformPass::DoRun()
{
	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buf->vbo);
	{
		Use shader(program);
		glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query); 
		glBeginTransformFeedback(objtype);
		glEnable(GL_RASTERIZER_DISCARD); 

		out->Draw(program);

		glDisable(GL_RASTERIZER_DISCARD); 
		glEndTransformFeedback();
		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN); 
		
		// performance loss of 15ms order
		TODO_W("Move this query to the *start* of the transform, use the old objects in the drawing phase.");
		glGetQueryObjectuiv(query, GL_QUERY_RESULT, &buf->count);
	}
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	swap(*buf, *out);
}

