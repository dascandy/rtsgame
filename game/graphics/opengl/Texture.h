#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class ShaderProgram;

class Texture {
public:
	Texture(int w, int h, int format = GL_RGBA8);
	virtual ~Texture();
	virtual void Reload();
	int width() { return w; }
	int height() { return h; }
	void SetContent(unsigned char *data);
	void SetContent(float *data);
	virtual void SetMipmapped(bool mipmapped);
	virtual void GenMipmap();
	virtual void SetAsAttachment(int i);
	bool Bind(ShaderProgram &prog, const std::string &name);
	unsigned int textureId;
	virtual void swap(Texture &other);
	unsigned char *read();
	void Clear();
	int w, h;
protected:
	int type;
	bool mipmap;
	int format;
	friend class Texture3d;
};

void swap(Texture &a, Texture &b);

#endif

