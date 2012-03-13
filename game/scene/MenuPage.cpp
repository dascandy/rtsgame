#include "MenuPage.h"

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
}

void MenuPage::setActive(bool active) {
	if (active == this->active) return;
	this->active = active;
	if (active) {
		objs[activeObj]->setColor(0xFFFF00);
	} else {
		objs[activeObj]->setColor(0xFFFFFF);
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

void MenuPage::KeyDown(int)
{
}

void MenuPage::KeyUp(int)
{
	
}

