#ifndef EDITPANEL_H
#define EDITPANEL_H

#include "IListener.h"
#include "Renderable.h"
#include "Toggle.h"
#include "Selector.h"

class EditPanel : public Renderable
				, public IListener<Selector>
				, public IListener<Toggle>
{
public:
	EditPanel();
private:
	Selector group;
	Selector subgroup;
	Selector item;
	Toggle gravity;
	Toggle light;
};

#endif

