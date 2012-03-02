#include "ResourceManager.h"
#include <IL/il.h>
#include "Texture.h"

class TextureHandler : public ResourceTypeHandler<Texture>, public ResourceStorer<Texture> {
private:
	int devilCode;
	const char *ext;
public:
	TextureHandler(int devilCode, const char *ext);
	Texture *load(Blob &b);
	Blob save(Texture *);
	const char *getExt() { return ext; }
};

static TextureHandler _instPng(IL_PNG, "png"), _instJpg(IL_JPG, "jpg"), instJpeg(IL_JPG, "jpeg"), instBmp(IL_BMP, "bmp");

TextureHandler::TextureHandler(int devilCode, const char *ext) 
: devilCode(devilCode)
, ext(ext)
{
	ResourceManager::Instance().RegisterResourceTypeHandler<Texture>(*this);
	ResourceManager::Instance().RegisterResourceStorer<Texture>(*this);
}

Texture *TextureHandler::load(Blob &b) {
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadL(devilCode, b.data, b.size);
	Texture *tex = new Texture(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_RGBA8, ilGetData();
	ilDeleteImages(1, &id);
	return tex;
}

Blob TextureHandler::save(Texture *b) {
	ilEnable(IL_FILE_OVERWRITE);
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	unsigned char *buf = b->read();
	ilTexImage(b->width, b->height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, buf);
	delete [] buf;
	const int maxwidth = 2048;
	const int maxheight = 2048;
	static unsigned char data[maxwidth * maxheight * 3];
	assert(ilSetData(data));
	imagedata.blob.size = ilSaveL(IL_PNG, data, maxwidth * maxheight * 3);
	ImageData imagedata;
	imagedata.type = IL_PNG;
	unsigned char* savedata = (unsigned char*)malloc(imagedata.blob.size);
	imagedata.blob.buf.reset(savedata);
	memcpy(savedata, data, imagedata.blob.size);
	ilDeleteImages(1, &id);
	return imagedata;
}


