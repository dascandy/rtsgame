#include "Texture.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ShaderProgram.h"
#include "swap.h"

std::map<int, Texture *> Texture::textures;

void ReadHdrFile(std::string name, float *buffer);

Texture::Texture(int w, int h, int format)
: textureId(0)
, w(w)
, h(h)
, mipmap(false)
, format(format)
{
	type = GL_TEXTURE_2D;
	Reload();
}

Texture::~Texture()
{
	textures[textureId] = 0;
	glDeleteTextures( 1, &textureId );
}

void Texture::Reload()
{
	textures[textureId] = 0;
	glDeleteTextures( 1, &textureId );
	glGenTextures( 1, &textureId );
	glBindTexture( type, textureId );
	textures[textureId] = this;
	glTexParameterf( type, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( type, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexImage2D(type, 0, format, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	SetMipmapped(mipmap);
	glBindTexture( type, 0);
}

void Texture::SetContent(unsigned char *data)
{
	glBindTexture( type, textureId );
	glTexImage2D(type, 0, format, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture( type, 0);
}

void Texture::SetContent(float *data)
{
	glBindTexture( type, textureId );
	glTexImage2D(type, 0, format, w, h, 0, GL_RGBA, GL_FLOAT, data);
	glBindTexture( type, 0);
}

unsigned char *Texture::read() {
	unsigned char *buffer = new unsigned char[w * h * 4];
	glBindTexture( type, textureId );
	glGetTexImage(type, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture( type, 0);
	return buffer;
}

void Texture::SetMipmapped(bool mipmapped)
{
	glBindTexture(type, textureId);
	mipmap = mipmapped;
	if (mipmap)
	{
		glTexParameterf( type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameterf( type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		GenMipmap();
	}
	else
	{
		glTexParameterf( type, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	glBindTexture( type, 0);
}

void Texture::GenMipmap()
{
	if (!mipmap) 
		return;

	glBindTexture(type, textureId);
	glGenerateMipmapEXT(type);
	glBindTexture( type, 0);
}

void Texture::SetAsAttachment(int i)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureId, 0);
}

bool Texture::Bind(ShaderProgram &prog, const std::string &name)
{
	if (!prog.SetTexture(name.c_str(), prog.curtex))
		return false;

	glActiveTexture(GL_TEXTURE0+prog.curtex);
	glBindTexture(type, textureId);
	prog.curtex++;
	return true;
}

void Texture::swap(Texture &o)
{
	if (type != o.type) 
		throw -1;
	if (format != o.format)
		throw -1;
	if (w != o.w)
		throw -1;
	if (h != o.h)
		throw -1;
	if (mipmap != o.mipmap)
		throw -1;

	::swap(o.textureId, textureId);
}

void swap(Texture &a, Texture &b)
{
	// invoke virtual function call mechanism
	a.swap(b);
}

