#include "Selector.h"
#include "debug.h"

Selector::Selector(float x, float y, IListener<Selector> &listener)
: left(*this)
, right(*this)
, text(15, "")
, listener(listener)
{

}

void Selector::Draw(ShaderProgram &)
{
	TODO("make");
}

void Selector::OnChange(Button *b)
{
	index += (b == &left ? -1 : +1);

	listener.OnChange(this);
}

void Selector::SetItemDisplayed(std::string item)
{
	text.SetText(item.c_str());
}

