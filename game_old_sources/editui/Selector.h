#ifndef SELECTOR_H
#define SELECTOR_H

#include "Renderable.h"
#include "Button.h"
#include "Label.h"
#include "IListener.h"
#include <vector>
#include <string>

class Selector : public Renderable, public IListener<Button>
{
public:
	Selector(float x, float y, IListener<Selector> &listener);
	void Draw(ShaderProgram &);
	void OnChange(Button *b);
	void SetItemDisplayed(std::string item);
	int index;
private:
	Button left, right;
	Label text;
	IListener<Selector> &listener;
};

#endif

