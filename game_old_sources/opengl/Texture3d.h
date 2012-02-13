#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "Texture.h"

class Texture3d : public Texture {
public:
	Texture3d(int w, int h, int d, int format = GL_RGBA);
	void Reload();
	virtual void SetContent(void *data);
	void SetAsAttachment(int i);
	int depth() { return d; }
	virtual void swap(Texture &other);
private:
	int d;
};

#endif

