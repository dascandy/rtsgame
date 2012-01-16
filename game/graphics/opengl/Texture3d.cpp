#include "Texture3d.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include "swap.h"

Texture3d::Texture3d(int w, int h, int d, int format)
: Texture(w, h, format)
, d(d)
{
	glDeleteTextures( 1, &textureId );
	type = GL_TEXTURE_3D;
	Reload();
}

void Texture3d::Reload()
{
	glDeleteTextures( 1, &textureId );
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_3D, textureId );
	glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	SetMipmapped(mipmap);
	glBindTexture( GL_TEXTURE_3D, 0);
}

void Texture3d::SetContent(void *data)
{
	glBindTexture( type, textureId );
	glTexImage3D(type, 0, format, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture( type, 0);
}

void Texture3d::SetAsAttachment(int i)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureId, 0);
}

void Texture3d::swap(Texture &o)
{
	Texture3d &ot = (Texture3d &)o;
	if (type != ot.type) 
		throw -1;
	if (format != ot.format)
		throw -1;
	if (w != ot.w)
		throw -1;
	if (h != ot.h)
		throw -1;
	if (d != ot.d)
		throw -1;
	if (mipmap != ot.mipmap)
		throw -1;

	::swap(ot.textureId, textureId);
}

