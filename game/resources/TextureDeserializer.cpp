#include "TextureDeserializer.h"
#include <SDL/SDL.h>
#include "Texture.h"
#include "debug.h"
#include "stream.h"
#include "ResourceFile.h"
#include "stb_image_aug.h"

TextureDeserializer::TextureDeserializer(std::string name)
: name(name)
{
}

Texture *TextureDeserializer::deserialize()
{
	TODO_W("Fix this to not use try/catch for resource file handling - precompile textures");
	try {
		return deserializeHdr(ResourceFile::open(name, ResourceFile::HdrTexture));
	} catch(std::exception) { try {
		return deserializeHdr(ResourceFile::open("OGA_" + name, ResourceFile::HdrTexture));
	} catch(std::exception) { try {
		return deserializePng(ResourceFile::open(name, ResourceFile::PngTexture));
	} catch(std::exception) { try {
		return deserializePng(ResourceFile::open("OGA_" + name, ResourceFile::PngTexture));
	} catch(std::exception) { try {
		return deserializeBmp(ResourceFile::open(name, ResourceFile::BmpTexture));
	} catch(std::exception) { try {
		return deserializeBmp(ResourceFile::open("OGA_" + name, ResourceFile::BmpTexture));
	} catch(std::exception) {
		LOG_WARN("Texture %s not found.", name.c_str());
		return NULL;
	}}}}}}
}

Texture *TextureDeserializer::deserializeHdr(stream in){
	unsigned char *buf = new unsigned char[in.length];
	in.read(buf, in.length);
	int x, y, components;
	float *target = (float *)stbi_hdr_load_from_memory(buf, in.length, &x, &y, &components, 4);
	Texture *t = new Texture(x, y, GL_RGBA16F);
	t->SetContent(target);
	stbi_image_free(target);
	return t;
}

Texture *TextureDeserializer::deserializeBmp(stream in) {
	unsigned char *buf = new unsigned char[in.length];
	in.read(buf, in.length);
	int x, y, components;
	unsigned char *target = stbi_bmp_load_from_memory(buf, in.length, &x, &y, &components, 4);
	Texture *t = new Texture(x, y);
	t->SetContent(target);
	stbi_image_free(target);
	return t;
}

Texture *TextureDeserializer::deserializePng(stream in)
{
	unsigned char *buf = new unsigned char[in.length];
	in.read(buf, in.length);
	int x, y, components;
	unsigned char *target = stbi_png_load_from_memory(buf, in.length, &x, &y, &components, 4);
	Texture *t = new Texture(x, y);
	t->SetContent(target);
	stbi_image_free(target);
	return t;
}

