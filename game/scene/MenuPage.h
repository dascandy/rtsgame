#ifndef MENUPAGE_H
#define MENUPAGE_H

#include "InputCallback.h"

class MenuPage : public InputCallback {
private:
	std::vector<MenuObject *> objs;
	bool active;
	int activeObj;
public:
	int x, y;
	Menu *menu;
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

