#ifndef TEXTUREHDL_H
#define TEXTUREHDL_H

#include "ResourceManager.h"
class Texture;

class TextureHandler : public ResourceTypeHandler<Texture>, public ResourceStorer<Texture> {
public:
	TextureHandler();
	Texture *load(Blob &b);
	Blob save(Texture *);
};

#endif


