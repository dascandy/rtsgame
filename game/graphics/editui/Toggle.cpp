#include "Toggle.h"
#include "debug.h"

Toggle::Toggle(IListener<Toggle> &listener, const char *name)
: toggle(*this)
, name(12, name)
, listener(listener)
{
}

void Toggle::Draw(ShaderProgram &prog)
{
	TODO("make this");
}

void Toggle::OnChange(Button *)
{
	listener.OnChange(this);
}

bool Toggle::IsOn()
{
	return toggle.on;
}

