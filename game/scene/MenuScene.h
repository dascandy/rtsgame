#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <vector>
#include <map>
#include <set>
#include "Renderable.h"

class MenuObject;

class MenuScene : public Renderable {
public:
	MenuScene();
	void events(unsigned int milliseconds);
	void add(MenuObject *);
	void remove(MenuObject *);
	void Draw(Res<ShaderProgram> &prog);
	void DrawPick(Res<ShaderProgram> &prog, std::vector<MenuObject *> &objs);
private:
	std::vector<MenuObject *> objects;
};

#endif

