#include "Texture.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ShaderProgram.h"
#include "swap.h"

std::map<int, Texture* > Texture::textures;

void ReadHdrFile(std::string name, float *buffer);

Texture::Texture(int w, int h, int format)
: textureId(0)
, w(w)
, h(h)
, mipmap(false)
, format(format)
, lastLoadedFloat(false)
{
	if (format == GL_R32F ||
		format == GL_RGBA32F) lastLoadedFloat = true;
	type = GL_TEXTURE_2D;
	Reload();
}

Texture::~Texture()
{
	textures.erase(textureId);
	glDeleteTextures( 1, &textureId );
}

void Texture::Reload()
{
	if (textureId)
		textures.erase(textureId);

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
	lastLoadedFloat = false;
}

void Texture::SetContent1ch(float *data)
{
	glBindTexture( type, textureId );
	glTexImage2D(type, 0, format, w, h, 0, GL_RED, GL_FLOAT, data);
	glBindTexture( type, 0);
	lastLoadedFloat = true;
}

void Texture::SetContent(float *data)
{
	glBindTexture( type, textureId );
	glTexImage2D(type, 0, format, w, h, 0, GL_RGBA, GL_FLOAT, data);
	glBindTexture( type, 0);
	lastLoadedFloat = true;
}

unsigned char *Texture::read() {
	if (lastLoadedFloat) {
		float *buf2 = new float[w*h*4];
		glBindTexture( type, textureId );
		glGetTexImage(type, 0, GL_RGBA, GL_FLOAT, buf2);
		glBindTexture( type, 0);
		float min = 0, max = 0;
		for (int i = 0; i < w*h*4; i+=4) {
			if (buf2[i] < min) min = buf2[i];
			if (buf2[i] > max) max = buf2[i];
		}
		unsigned char *buffer = new unsigned char[w * h * 4];
		for (int i = 0; i < w*h*4; i+=4) {
			buffer[i] = (unsigned char)((buf2[i] - min) * 255.45 / max);
			buffer[i+1] = buffer[i];
			buffer[i+2] = buffer[i];
			buffer[i+3] = 255;
		}
		delete [] buf2;
		return buffer;
	} else {
		unsigned char *buffer = new unsigned char[w * h * 4];
		glBindTexture( type, textureId );
		glGetTexImage(type, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glBindTexture( type, 0);
		return buffer;
	}
}

float *Texture::readF() {
	float *buf2 = new float[w*h];
	glBindTexture( type, textureId );
	glGetTexImage( type, 0, GL_RED, GL_FLOAT, buf2);
	glBindTexture( type, 0);
	return buf2;
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

bool Texture::Bind(Res<ShaderProgram> &prog, const std::string &name)
{
	if (!prog->SetTexture(name.c_str(), prog->curtex))
		return false;

	glActiveTexture(GL_TEXTURE0+prog->curtex);
	glBindTexture(type, textureId);
	prog->curtex++;
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

