#include "MenuScene.h"
#include "MenuObject.h"
#include "ShaderProgram.h"

MenuScene::MenuScene() {
}

void MenuScene::events(unsigned int milliseconds) {
}

void MenuScene::add(MenuObject *obj) {
	objects.push_back(obj);
}

void MenuScene::remove(MenuObject *obj) {
	TODO("Add remove functionality");
//	objects.erase(objects.find(obj));
}

void MenuScene::Draw(Res<ShaderProgram> &prog) {
	for (std::vector<MenuObject *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->Draw(prog);
	}
}

void MenuScene::DrawPick(Res<ShaderProgram> &prog, std::vector<MenuObject *> &objs) {
	for (std::vector<MenuObject *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if (!(*it)->clickable()) continue;

		objs.push_back(*it);
		prog->Set("pickid", objs.size());
		(*it)->Draw(prog);
	}
}

