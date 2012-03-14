#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <vector>
#include "InputCallback.h"

class MenuAction;
class MenuObject;
class Menu;

class MenuPage : public InputCallback {
	bool active;
	unsigned int activeObj;
public:
	std::vector<MenuObject *> objs;
	int x, y;
	Menu *menu;
	MenuAction *OnCancel;
	MenuPage(Menu *menu, int x, int y);
	void add(MenuObject *obj);
	void setActive(bool active);

	void MouseMoveX(int);
	void MouseMoveY(int);
	void MouseScrollUp();
	void MouseScrollDown();
	void MouseButtonUp(int);
	void MouseButtonDown(int);
	void KeyDown(int);
	void KeyUp(int);
};

#endif

