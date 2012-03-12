#include "Menu.h"
#include "MenuObject.h"

Menu::Menu(RenderTarget &rt) 
: rt(rt)
, font(ResourceManager::Instance().getResource<Font>("pfennig"))
, created(false)
, pass(ResourceManager::Instance().getResource<ShaderProgram>("menu"), scene, rt)
{
}

void Menu::update(int ms) {

}

void Menu::create() {
	char logo[2] = {0};
	logo[0] = 31;
	scene.add(new MenuObject(font->getText(logo, 0.4f), vec3(0.9f, -0.6f, 0)));
	scene.add(new MenuObject(font->getText("Single player", 0.2f), vec3(0.9f, -0.4f, 0)));
	scene.add(new MenuObject(font->getText("Multiplayer", 0.2f), vec3(0.9f, -0.1f, 0)));
	scene.add(new MenuObject(font->getText("Settings", 0.2f), vec3(0.9f, 0.2f, 0)));
	scene.add(new MenuObject(font->getText("Credits", 0.2f), vec3(0.9f, 0.5f, 0)));
	scene.add(new MenuObject(font->getText("Exit", 0.2f), vec3(0.9f, 0.8f, 0)));
}

void Menu::run() {
	if (!created && font.isLoaded() && font->texture.isLoaded()) {
		create();
		pass.AddTexture("font", font->texture);
		created = true;
	}

	if (created)
		pass.Run();
}

void Menu::MouseMoveX(int)
{

}

void Menu::MouseMoveY(int)
{
	
}

void Menu::MouseScrollUp()
{
	
}

void Menu::MouseScrollDown()
{
	
}

void Menu::MouseButtonUp(int)
{
	
}

void Menu::MouseButtonDown(int)
{
	
}

void Menu::KeyDown(int)
{
}

void Menu::KeyUp(int)
{
	
}


