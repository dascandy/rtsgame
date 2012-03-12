#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>
#include <GL/glew.h>
#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"

class Font {
public:
	Res<Model> getText(const char *text, float size = 12.0f, float maxWidthPerLine = 99999999);
	~Font();
	static const char *getDirName() { return "fonts"; }
	Res<Texture> texture;
private:
	static std::map<std::string, Font* > fonts;
	friend class FontReader;
	friend class FontWriter;
	friend class FontWebClient;
	Font(const char *name, Res<Texture> texture, const char *texturename);
	struct entry {
		entry (int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		int x, y, w, h;
	};
	std::map<char, entry> entries;
	const char *fontname;
	const char *texturename;
};

#endif

