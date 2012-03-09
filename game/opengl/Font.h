#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>
#include <GL/glew.h>
#include "ResourceManager.h"
#include "Texture.h"

class Font {
public:
	static const char *getDirName() { return "fonts"; }
	static std::map<std::string, Font* > fonts;


private:
	friend class FontHandler;
	friend class FontStorer;
	friend class FontWebClient;
	Font(Res<Texture> texture) : texture(texture) {}
	struct entry {
		entry (int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		int x, y, w, h;
	};
	std::map<char, entry> entries;
	Res<Texture> texture;
};

#endif

