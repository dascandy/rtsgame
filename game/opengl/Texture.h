#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <string>
#include <GL/glew.h>
#include "ResourceManager.h"

class ShaderProgram;

class Texture {
public:
	Texture(int w, int h, int format = GL_RGBA8);
	virtual ~Texture();
	virtual void Reload();
	static const char *getDirName() { return "textures"; }
	int width() { return w; }
	int height() { return h; }
	void SetContent(unsigned char *data);
	void SetContent1ch(float *data);
	void SetContent(float *data);
	void SetSubimage(int x, int y, int w, int h, float *data);
	void SetSubimage(int x, int y, int w, int h, int *data);
	void SetSubimage(int x, int y, int w, int h, unsigned char *data);
	virtual void SetMipmapped(bool mipmapped);
	virtual void GenMipmap();
	virtual void SetAsAttachment(int i);
	bool Bind(Res<ShaderProgram> &prog, const std::string &name);
	unsigned int textureId;
	virtual void swap(Texture &other);
	unsigned char *read();
	float *readF();
	void Clear();
	int w, h;
protected:
	int type;
	bool mipmap;
	int format;
	friend class Texture3d;
	bool lastLoadedFloat;
public:
	static std::map<int, Texture* > textures;
};

void swap(Texture &a, Texture &b);

#endif

