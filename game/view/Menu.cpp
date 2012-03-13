#include "Menu.h"
#include "MenuObject.h"

Menu::Menu(RenderTarget &rt) 
: rt(rt)
, font(ResourceManager::Instance().getResource<Font>("pfennig"))
, created(false)
, pass(ResourceManager::Instance().getResource<ShaderProgram>("menu"), scene, rt)
{
	projection = ortho( 0.0f, 1000.0f, 1000.0f * rt.height / rt.width, 0.0f,-5.0f, 5.0f);
	pass.Register("mat_vp", view_proj);
	float rel_height = 1024.0f * rt.height / rt.width;
	viewposition = vec3(1000, 1000 + (1024 - rel_height) / 2, 0);
}

void Menu::update(int ms) {

}

void Menu::create() {
	// main menu, base 1024/1024
	char logo[2] = {0}; logo[0] = 31;
	scene.add(new MenuObject(font->getText(logo, 100.0f), vec3(1200, 1250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Single player", 50.0f), vec3(1100, 1375, 0)));
	scene.add(new MenuObject(font->getText("Multiplayer", 50.0f), vec3(1100, 1450, 0)));
	scene.add(new MenuObject(font->getText("Settings", 50.0f), vec3(1100, 1525, 0)));
	scene.add(new MenuObject(font->getText("Credits", 50.0f), vec3(1100, 1600, 0)));
	scene.add(new MenuObject(font->getText("Exit", 50.0f), vec3(1100, 1675, 0)));

	// single-player menu
	scene.add(new MenuObject(font->getText(logo, 20.0f), vec3(1800, 250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Single player", 100.0f), vec3(1100, 250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("New game", 50.0f), vec3(1100, 375, 0)));
	scene.add(new MenuObject(font->getText("Continue", 50.0f), vec3(1100, 450, 0)));
	scene.add(new MenuObject(font->getText("Load game", 50.0f), vec3(1100, 525, 0)));
	scene.add(new MenuObject(font->getText("Save game", 50.0f), vec3(1100, 600, 0)));

	// multi-player menu
	scene.add(new MenuObject(font->getText(logo, 20.0f), vec3(2800, 1250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Multiplayer", 100.0f), vec3(2100, 1250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Play hosted", 50.0f), vec3(2100, 1375, 0)));
	scene.add(new MenuObject(font->getText("Join LAN game", 50.0f), vec3(2100, 1450, 0)));
	scene.add(new MenuObject(font->getText("Host LAN game", 50.0f), vec3(2100, 1525, 0)));

	// exit menu
	scene.add(new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Exit", 100.0f), vec3(100, 250, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Are you sure you want to quit?", 60.0f), vec3(100, 400, 0), quat(), false));
	scene.add(new MenuObject(font->getText("Yes", 50.0f), vec3(200, 575, 0)));
	scene.add(new MenuObject(font->getText("No", 50.0f), vec3(700, 575, 0)));

	scene.add(new MenuObject(font->getText("MEH", 500.0f), vec3(-250, -250, 0)));
}

void Menu::run() {
	if (!created && font.isLoaded() && font->texture.isLoaded()) {
		create();
		pass.AddTexture("font", font->texture);
		created = true;
	}
	mat4 view;
	view[3] = vec4(-viewposition, 1);

	view_proj = projection * view;

	if (created && font.isLoaded() && font->texture.isLoaded())
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


