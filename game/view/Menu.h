#ifndef MENU_H
#define MENU_H

#include "InputCallback.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "ResourceManager.h"
#include "Font.h"
#include "glm.h"
#include "MenuScene.h"
#include "TextureRenderPass.h"
#include "Var.h"

class RenderTarget;
class MenuPage;
class Game;

class Menu : public InputCallback
{
public:
	Menu(Game *gw, RenderTarget &rt);
	void create();
	void update(int ms);
	void run();
	VarNum<int> *time;
	void setCurrentPage(MenuPage *page);
	InputCallback *getActivePage();
	void QuitGame();
	void setIsInGame(bool inGame);
private:
	void toLoadGame();
	void toSaveGame();
	void addSavedGameList(bool save);
	void enterNameForSave();
	Game *game;
	RenderTarget &rt;
	Res<Font> font;
	MenuScene scene;
	MenuPage *activePage;
	Var<vec3> *viewposition;
	mat4 projection, view_proj;
	bool created;
	TextureRenderPass pass;

	MenuPage *menuMainMenu; 
	MenuPage *menuSinglePlayer;
	MenuPage *menuLoadSave;
	MenuPage *menuExit;
	MenuPage *menuMultiplayer;
	MenuPage *menuAudioSettings;
	MenuPage *menuVideoSettings;
	MenuPage *menuGraphicsSettings;
	MenuPage *menuCredits;
	MenuPage *menuJoinGame;
	MenuPage *menuHostGame;
	MenuPage *menuConnecting;
	MenuPage *menuHostedGame;
	MenuPage *menuJoinedGame;
	MenuPage *menuGameStarting;
	MenuPage *menuConnectionLost;
	MenuPage *menuIngame;

	void createMainMenu(); 
	void createSinglePlayer();
	void createLoadSave();
	void createExit();
	void createMultiplayer();
	void createAudioSettings();
	void createVideoSettings();
	void createGraphicsSettings();
	void createCredits();
	void createJoinGame();
	void createHostGame();
	void createConnecting();
	void createHostedGame();
	void createJoinedGame();
	void createGameStarting();
	void createConnectionLost();
	void createIngame();
};

#endif

