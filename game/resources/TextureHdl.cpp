#include "ResourceManager.h"
#include <IL/il.h>
#include "Texture.h"
class Texture;

class TextureHandler : public ResourceTypeHandler<Texture>, public ResourceStorer<Texture> {
public:
	TextureHandler();
	Texture *load(Blob &b);
	Blob save(Texture *);
	const char *getExt() { return "png"; }
};

#endif



static TextureHandler _instance;

TextureHandler::TextureHandler() {
	ResourceManager::Instance().RegisterResourceTypeHandler<Texture>(*this);
	ResourceManager::Instance().RegisterResourceStorer<Texture>(*this);
}

Texture *TextureHandler::load(Blob &b) {
: texture(texture)
{
ilGenImages(1, &id);
ilBindImage(id);
ilTexImage(texture.width, texture.height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, 0);
}
void load(ImageData& imagedata)
{
ilBindImage(id);
texture.Bind(0);
ilLoadL(imagedata.type, imagedata.blob.buf.get(), imagedata.blob.size);
assert(ilGetData() != 0);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
}
ImageData save()
{
const int maxwidth = 2048;
const int maxheight = 2048;
static unsigned char data[maxwidth * maxheight * 3];
ilBindImage(id);
texture.Bind(0);
glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &data);
assert(ilSetData(data));
ImageData imagedata;
imagedata.type = IL_PNG;
imagedata.blob.size = ilSaveL(IL_PNG, data, maxwidth * maxheight * 3);
unsigned char* savedata = (unsigned char*)malloc(imagedata.blob.size);
imagedata.blob.buf.reset(savedata);
memcpy(savedata, data, imagedata.blob.size);
return imagedata;
}
~Image()
{
ilDeleteImages(1, &id);
}

Blob TextureHandler::save(Texture *b) {

}


