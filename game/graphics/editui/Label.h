#ifndef LABEL_H
#define LABEL_H

#include "Renderable.h"

class Texture;

class Label : public Renderable
{
public:
	Label(int charsX, const char *text = "");
	void Draw(ShaderProgram &);
	void SetText(const char *txt);
	float x, y, w, h;
private:
	int charsX;
	char *buf;
	static Texture *fontTex;
};

#endif

