#include "RenderTarget.h"
#include "Window.h"
#include "Texture.h"
#include <GL/glew.h>
#include "debug.h"
static const unsigned int buffers[] = 
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15
};

RenderTarget::RenderTarget(int width, int height, bool depth)
: depth(depth)
, width(width)
, height(height)
, vx(0)
, vy(0)
, vw(width)
, vh(height)
, fbo(0)
, rb(0)
, blendmode(Overwrite)
{
}

void RenderTarget::AddTarget(Texture &target)
{
	if (targets.size())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	else
	{
		width = target.width();
		height = target.height();

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		if (depth)
		{
			glGenRenderbuffers(1, &rb);
			glBindRenderbuffer(GL_RENDERBUFFER, rb);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
		}
	}

	target.SetAsAttachment(targets.size());
	int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(err)
	{
	case GL_FRAMEBUFFER_COMPLETE: break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_UNSUPPORTED:printf("GL_FRAMEBUFFER_UNSUPPORTED\n"); DebugBreak(); break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:printf("GL_INVALID_FRAMEBUFFER_OPERATION\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB:printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB:printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB\n"); DebugBreak(); break;
	default: printf("%d\n", err); DebugBreak(); break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	targets.push_back(&target);
}

RenderTarget::~RenderTarget()
{
	glDeleteRenderbuffers(1, &rb);
	glDeleteFramebuffers(1, &fbo);
}

static int cvx = 0, cvy = 0, cvw = 0, cvh = 0;

void RenderTarget::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (targets.size())
	{
		glDrawBuffers(targets.size(), buffers);
		for (size_t i = 0; i < targets.size(); i++)
		{
			targets[i]->SetAsAttachment(i);
		}
	}

	if (cvx != vx || cvy != vy || cvw != vw || cvh != vh)
	{
		cvx = vx;
		cvy = vy;
		cvw = vw;
		cvh = vh;
		glViewport(vx, vy, vw, vh);
	}

	int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(err)
	{
	case GL_FRAMEBUFFER_COMPLETE: break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_UNSUPPORTED:printf("GL_FRAMEBUFFER_UNSUPPORTED\n"); DebugBreak(); break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:printf("GL_INVALID_FRAMEBUFFER_OPERATION\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB:printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB\n"); DebugBreak(); break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB:printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB\n"); DebugBreak(); break;
	default: printf("%d\n", err); DebugBreak(); break;
	}

	if (depth)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	static BlendMode currentBlendFunc = Overwrite;
	switch(blendmode)
	{
	case Overwrite:
		glDisable(GL_BLEND);
		break;
	case Alphablend:
		if (currentBlendFunc != Alphablend) 
		{
			currentBlendFunc = Alphablend;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glEnable(GL_BLEND);
		break;
	case Additive:
		if (currentBlendFunc != Additive) 
		{
			currentBlendFunc = Additive;
			glBlendFunc(GL_ONE, GL_ONE);
		}
		glEnable(GL_BLEND);
		break;
	}
}

void RenderTarget::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (cvx != vx || cvy != vy || cvw != vw || cvh != vh)
	{
		cvx = vx;
		cvy = vy;
		cvw = vw;
		cvh = vh;
		glViewport(vx, vy, vw, vh);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::ClearDepth()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (cvx != vx || cvy != vy || cvw != vw || cvh != vh)
	{
		cvx = vx;
		cvy = vy;
		cvw = vw;
		cvh = vh;
		glViewport(vx, vy, vw, vh);
	}
	glClear(GL_DEPTH_BUFFER_BIT);
}

