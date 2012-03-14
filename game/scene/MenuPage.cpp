#include "MenuPage.h"
#include "MenuObject.h"
#include "Menu.h"
#include "MenuAction.h"

#define FADETIME 100
#define ANIMTIME 500

std::vector<MenuObject *> objs;
bool active;
int activeObj;
int x, y;

MenuPage::MenuPage(Menu *menu, int x, int y) 
: active(false)
, activeObj(0)
, x(x)
, y(y)
, menu(menu)
{
}

void MenuPage::add(MenuObject *obj) {
	objs.push_back(obj);
	obj->location += vec3(x, y, 0);
}

void MenuPage::setActive(bool active) {
	if (active == this->active) return;
	this->active = active;
	if (active) {
		activeObj = 0;
		while (activeObj < objs.size() && objs[activeObj]->OnActivate == 0) activeObj++;
		if (activeObj < objs.size() && objs[activeObj]->OnActivate != 0) 
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, cosinterp(new VarNum<Color>(0xFFFF), new VarNum<Color>(0xFF), menu->time, ANIMTIME),
				menu->time, FADETIME, **(menu->time));
	} else {
		if (activeObj < objs.size() && objs[activeObj]->OnActivate != 0) 
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, new VarNum<Color>(0xFFFFFF), menu->time, FADETIME, **(menu->time));
	}
}

void MenuPage::MouseMoveX(int)
{

}

void MenuPage::MouseMoveY(int)
{
	
}

void MenuPage::MouseScrollUp()
{
	
}

void MenuPage::MouseScrollDown()
{
	
}

void MenuPage::MouseButtonUp(int)
{
	
}

void MenuPage::MouseButtonDown(int)
{
	
}

void MenuPage::KeyDown(int key)
{
	if (key == 13)
	{
		if (activeObj < objs.size() && objs[activeObj]->OnActivate != 0) 
		{
			(*(objs[activeObj]->OnActivate))();
		}
	}
	else if (key == 27) 
	{
		if (OnCancel)
			(*OnCancel)();
	}
	else if (key == 1073741906) 
	{
		if (activeObj < objs.size() && objs[activeObj]->OnActivate != 0) 
		{
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, new VarNum<Color>(0xFFFFFF), menu->time, FADETIME, **(menu->time));
			do {
				if (activeObj == 0) activeObj = objs.size() - 1;
				else activeObj--;
			} while (objs[activeObj]->OnActivate == 0);
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, cosinterp(new VarNum<Color>(0xFFFF), new VarNum<Color>(0xFF), menu->time, ANIMTIME),
				menu->time, FADETIME, **(menu->time));
		}
	} else if (key == 1073741905) {
		if (activeObj < objs.size() && objs[activeObj]->OnActivate != 0)
		{
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, new VarNum<Color>(0xFFFFFF), menu->time, FADETIME, **(menu->time));
			do {
				activeObj++;
				if (activeObj >= objs.size()) activeObj = 0;
			} while (objs[activeObj]->OnActivate == 0);
			objs[activeObj]->color = fadesharp(objs[activeObj]->color, cosinterp(new VarNum<Color>(0xFFFF), new VarNum<Color>(0xFF), menu->time, ANIMTIME),
				menu->time, FADETIME, **(menu->time));
		}
	}
}

void MenuPage::KeyUp(int key)
{
	
}

