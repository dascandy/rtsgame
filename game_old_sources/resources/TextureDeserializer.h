#ifndef TEXTUREDESERIALIZER_H
#define TEXTUREDESERIALIZER_H

#include <string>
class Texture;
class stream;

class TextureDeserializer
{
public:
	TextureDeserializer(std::string name);
	Texture *deserialize();
private:
	Texture *deserializeHdr(stream file);
	Texture *deserializeBmp(stream file);
	Texture *deserializePng(stream file);
	std::string name;
};

#endif

