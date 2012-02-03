#ifndef BUTTON_H
#define BUTTON_H

#include "Renderable.h"
#include "Texture.h"
class Model;
#include "IListener.h"

class Button : public Renderable
{
public:
	Button(IListener<Button> &listener);
	void Draw(ShaderProgram &);
	bool on;
	float x, y, w, h;
private:
	Texture tOn, tOff;
	Model &quad;
	IListener<Button> &listener;
};

#endif

