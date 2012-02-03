#ifndef TOGGLE_H
#define TOGGLE_H

#include "Renderable.h"
#include "Button.h"
#include "Label.h"
#include "IListener.h"

class Toggle : public Renderable, public IListener<Button>
{
public:
	Toggle(IListener<Toggle> &listener, const char *name);
	void Draw(ShaderProgram &);
	void OnChange(Button *button);
	bool IsOn();
private:
	Button toggle;
	Label name;
	IListener<Toggle> &listener;
};

#endif

