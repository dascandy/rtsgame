#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <vector>

class Texture;

class RenderTarget
{
public:
	RenderTarget(int width, int height, bool depth = true);
	void Clear();
	void ClearDepth();
	void Activate();
	void AddTarget(Texture &target);
	~RenderTarget();
	int width, height;
	bool depth;
	int vx, vy, vw, vh;
	enum BlendMode {
		Overwrite,
		Alphablend,
		Additive
	} blendmode;
private:
	unsigned int fbo, rb;
	std::vector<Texture *> targets;
};

#endif

