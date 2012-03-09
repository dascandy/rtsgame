#include "ResourceManager.h"
#include <IL/il.h>
#include "Texture.h"

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

class TextureWriter : public ResourceStorer<Texture> {
private:
	int devilCode;
	const char *ext;
public:
	TextureWriter(int devilCode, const char *ext);
	Blob save(Texture *);
	const char *getExt() { return ext; }
};

class TextureReader : public ResourceTypeHandler<Texture> {
private:
	int devilCode;
	const char *ext;
public:
	TextureReader(int devilCode, const char *ext);
	Texture *load(Blob &b, const char *name);
	const char *getExt() { return ext; }
};

static TextureReader _instJpg(IL_JPG, "jpg"), instJpeg(IL_JPG, "jpeg"), instBmp(IL_BMP, "bmp"), _instPng(IL_PNG, "png");
static TextureWriter _writerPng(IL_PNG, "png");

TextureReader::TextureReader(int devilCode, const char *ext) 
: devilCode(devilCode)
, ext(ext)
{
	ResourceManager::Instance().RegisterResourceTypeHandler<Texture>(*this);
}

Texture *TextureReader::load(Blob &b, const char *) {
	ILuint id;
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadL(devilCode, b.data, b.size);
	Texture *tex = new Texture(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_RGBA8);
	tex->SetContent(ilGetData());
	ilDeleteImages(1, &id);
	return tex;
}

TextureWriter::TextureWriter(int devilCode, const char *ext) 
: devilCode(devilCode)
, ext(ext)
{
	ResourceManager::Instance().RegisterResourceStorer<Texture>(*this);
}

Blob TextureWriter::save(Texture *b) {
	ILuint id;
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1, &id);
	ilBindImage(id);
	int maxSize = b->width() * b->height() * 4 + 5000;
	unsigned char *buf = b->read();
	ilTexImage(b->width(), b->height(), 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, buf);
	delete [] buf;
	char *buffer = new char[maxSize];
	size_t actualSize = ilSaveL(IL_PNG, buffer, maxSize);
	ilDeleteImages(1, &id);
	return Blob(actualSize, buffer);
}

