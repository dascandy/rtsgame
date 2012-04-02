#include "Font.h"
#include <stdlib.h>
#include "Model.h"
#include "ResourceManager.h"

std::map<std::string, Font* > Font::fonts;

Res<Model> Font::getText(const char *text, float size, float maxWidthPerLine) {
	int count = strlen(text);
	Model::vertex *vtxs = new Model::vertex[count * 6];
	Model::vertex *cp = vtxs;
	float x = 0, y = 0, z = 0;
	while (*text) {
		if (entries.find(*text) == entries.end()) { text++; continue; }
		Font::entry &ch = entries.find(*text)->second;
		text++;
		float texW = (ch.w / 1024.0f), texH = (ch.h / 1024.0f);
		float texX = (ch.x / 1024.0f), texY = ((1024 - ch.y - ch.h) / 1024.0f);
		float relW = texW / texH * size, relH = size;

		if (x + relW > maxWidthPerLine) { x = 0; y += relH; }

		cp->x = x;        cp->y = y + relH; cp->z = z; cp->s0 = (unsigned short)((texX) * 65535);        cp->t0 = (unsigned short)((texY) * 65535);        cp++;
		cp->x = x + relW; cp->y = y + relH; cp->z = z; cp->s0 = (unsigned short)((texX + texW) * 65535); cp->t0 = (unsigned short)((texY) * 65535);        cp++;
		cp->x = x + relW; cp->y = y;        cp->z = z; cp->s0 = (unsigned short)((texX + texW) * 65535); cp->t0 = (unsigned short)((texY + texH) * 65535); cp++;
		cp->x = x + relW; cp->y = y;        cp->z = z; cp->s0 = (unsigned short)((texX + texW) * 65535); cp->t0 = (unsigned short)((texY + texH) * 65535); cp++;
		cp->x = x;        cp->y = y;        cp->z = z; cp->s0 = (unsigned short)((texX) * 65535);        cp->t0 = (unsigned short)((texY + texH) * 65535); cp++;
		cp->x = x;        cp->y = y + relH; cp->z = z; cp->s0 = (unsigned short)((texX) * 65535);        cp->t0 = (unsigned short)((texY) * 65535);        cp++;

		x += relW;
	}

	return new Model(count*6, vtxs);
}

Font::~Font() {
	free((void *)fontname);
	free((void *)texturename);	
}

Font::Font(const char *fontname, Res<Texture> texture, const char *texturename) 
: texture(texture)
, fontname(strdup(fontname))
, texturename(strdup(texturename))
{
	fonts.insert(std::make_pair(fontname, this));
}

