#include "Menu.h"
#include "MenuObject.h"
#include "MenuPage.h"
#include "MenuAction.h"
#include "Var.h"
#include "Game.h"

Menu::Menu(Game *game, RenderTarget &rt) 
: game(game)
, rt(rt)
, font(ResourceManager::Instance().getResource<Font>("pfennig"))
, time(new VarNum<int>(0))
, created(false)
, pass(ResourceManager::Instance().getResource<ShaderProgram>("menu"), scene, rt)
, activePage(0)
{
	projection = ortho( 0.0f, 1000.0f, 1000.0f * rt.height / rt.width, 0.0f,-5.0f, 5.0f);
	pass.Register("mat_vp", view_proj);
	float rel_height = 1000.0f * rt.height / rt.width;
	viewposition = new VarNum<vec3>(vec3(1000, 1000 + (1000 - rel_height) / 2, 0));

	menuMainMenu = new MenuPage(this, 1000, 1000); 
	menuSinglePlayer = new MenuPage(this, 1000, 0);
	menuLoadSave = new MenuPage(this, 0, 0);
	menuExit = new MenuPage(this, 0, 1000);
	menuMultiplayer = new MenuPage(this, 2000, 1000);
	menuAudioSettings = new MenuPage(this, 1000, 2000);
	menuVideoSettings = new MenuPage(this, 1000, 3000);
	menuGraphicsSettings = new MenuPage(this, 1000, 4000);
	menuCredits = new MenuPage(this, 0, 2000);
	menuJoinGame = new MenuPage(this, 2000, 2000);
	menuHostGame = new MenuPage(this, 3000, 0);
	menuConnecting = new MenuPage(this, 3000, 1000);
	menuHostedGame = new MenuPage(this, 4000, 1000);
	menuJoinedGame = new MenuPage(this, 3000, 2000);
	menuGameStarting = new MenuPage(this, 4000, 2000);
	menuConnectionLost = new MenuPage(this, 2000, 0);
	menuIngame = new MenuPage(this, 2000, 3000);

	activePage = menuMainMenu;
}

void Menu::update(int ms) {
	*time = **time + ms;
}

void Menu::run() {
	if (!created && font.isLoaded() && font->texture.isLoaded()) {
		create();
		pass.AddTexture("font", font->texture);
		created = true;
		menuMainMenu->setActive(true);
	}

	mat4 view;
	view[3] = vec4(-(**viewposition), 1);

	view_proj = projection * view;

	if (created && font.isLoaded() && font->texture.isLoaded())
	{
		rt.Clear();
		pass.Run();
	}
}

InputCallback *Menu::getActivePage() 
{
	return activePage;
}

void Menu::setCurrentPage(MenuPage *page) {
	activePage->setActive(false);
	activePage = page;
	activePage->setActive(true);
	float rel_height = 1000.0f * rt.height / rt.width;
	viewposition = fade(viewposition, new VarNum<vec3>(vec3(page->x, page->y + (1000 - rel_height) / 2, 0)), time, 500, **time);
}

void Menu::setIsInGame(bool inGame) {
	delete menuAudioSettings->OnCancel;
	menuAudioSettings->OnCancel = new SetMenuAction(this, inGame ? menuIngame : menuMainMenu);
	delete menuVideoSettings->OnCancel;
	menuVideoSettings->OnCancel = new SetMenuAction(this, inGame ? menuIngame : menuMainMenu);
	delete menuGraphicsSettings->OnCancel;
	menuGraphicsSettings->OnCancel = new SetMenuAction(this, inGame ? menuIngame : menuMainMenu);
	activePage = inGame ? menuIngame : menuMainMenu;
}

void Menu::QuitGame() {
	throw 1;
}

void Menu::toLoadGame() {
	MenuObject *title = menuLoadSave->objs[1];
	title->model = font->getText("Load game", 100.0f);
	addSavedGameList(false);
	setCurrentPage(menuLoadSave);
}

void Menu::toSaveGame() {
	MenuObject *title = menuLoadSave->objs[1];
	title->model = font->getText("Save game", 100.0f);
	addSavedGameList(true);
	setCurrentPage(menuLoadSave);
}

void Menu::addSavedGameList(bool save) {
	int x = 100, y = 375;

	// TODO: add actual savegames
	for (int i = 0; i > 1; i++) {
		MenuObject *savegame = new MenuObject(font->getText("someName", 30.0f), vec3(x, y, 0));
		menuLoadSave->add(savegame);
		scene.add(savegame);
		if (save)
			savegame->OnActivate = new ArbitraryAction(game, &Game::SaveGame, "SomeName");
		else
			savegame->OnActivate = new ArbitraryAction(game, &Game::LoadGame, "SomeName");

		y += 50;
		if (y == 675) { y = 375; x = 600; }
	}

	if (save) {
		MenuObject *newsavegame = new MenuObject(font->getText("<new savegame>", 30.0f), vec3(x, y, 0));
		menuLoadSave->add(newsavegame);
		scene.add(newsavegame);
		newsavegame->OnActivate = new ArbitraryAction(this, &Menu::enterNameForSave);
	}
}

void Menu::enterNameForSave() {
	TODO("Add naming your save files");
}

void Menu::create() {
	createMainMenu(); 
	createSinglePlayer();
	createExit();
	createMultiplayer();
	createLoadSave();
	createAudioSettings();
	createVideoSettings();
	createGraphicsSettings();
	createCredits();
	createJoinGame();
	createHostGame();
	createConnecting();
	createHostedGame();
	createJoinedGame();
	createGameStarting();
	createConnectionLost();
	createIngame();
}

void Menu::createMainMenu() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 100.0f), vec3(200, 250, 0)); 
	scene.add(mainLogo); 
	menuMainMenu->add(mainLogo);

	MenuObject *singlePlayer = new MenuObject(font->getText("Single player", 50.0f), vec3(100, 375, 0)); 
	scene.add(singlePlayer); 
	menuMainMenu->add(singlePlayer);
	singlePlayer->OnActivate = new SetMenuAction(this, menuSinglePlayer);
	
	MenuObject *multiplayer = new MenuObject(font->getText("Multiplayer", 50.0f), vec3(100, 450, 0)); 
	scene.add(multiplayer); 
	menuMainMenu->add(multiplayer);
	multiplayer->OnActivate = new SetMenuAction(this, menuMultiplayer);

	MenuObject *settings = new MenuObject(font->getText("Settings", 50.0f), vec3(100, 525, 0)); 
	scene.add(settings); 
	menuMainMenu->add(settings);
	settings->OnActivate = new SetMenuAction(this, menuAudioSettings);

	MenuObject *credits = new MenuObject(font->getText("Credits", 50.0f), vec3(100, 600, 0)); 
	scene.add(credits); 
	menuMainMenu->add(credits);
	credits->OnActivate = new SetMenuAction(this, menuCredits);

	MenuObject *exit = new MenuObject(font->getText("Exit", 50.0f), vec3(100, 675, 0)); 
	scene.add(exit); 
	menuMainMenu->add(exit);
	exit->OnActivate = new SetMenuAction(this, menuExit);

	menuMainMenu->OnCancel = new SetMenuAction(this, menuExit);
}

void Menu::createSinglePlayer() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuSinglePlayer->add(mainLogo);

	MenuObject *singlePlayer = new MenuObject(font->getText("Single player", 100.0f), vec3(100, 250, 0)); 
	scene.add(singlePlayer); 
	menuSinglePlayer->add(singlePlayer);
	
	MenuObject *newgame = new MenuObject(font->getText("New game", 50.0f), vec3(100, 450, 0)); 
	scene.add(newgame); 
	menuSinglePlayer->add(newgame);
	newgame->OnActivate = new ArbitraryAction(game, &Game::NewGame);

	MenuObject *moContinue = new MenuObject(font->getText("Continue", 50.0f), vec3(100, 525, 0)); 
	scene.add(moContinue); 
	menuSinglePlayer->add(moContinue);
	moContinue->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *load = new MenuObject(font->getText("Load game", 50.0f), vec3(100, 600, 0)); 
	scene.add(load); 
	menuSinglePlayer->add(load);
	load->OnActivate = new ArbitraryAction(this, &Menu::toLoadGame);

	MenuObject *save = new MenuObject(font->getText("Save game", 50.0f), vec3(100, 675, 0)); 
	scene.add(save); 
	menuSinglePlayer->add(save);
	save->OnActivate = new ArbitraryAction(this, &Menu::toSaveGame);

	menuSinglePlayer->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createMultiplayer() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuMultiplayer->add(mainLogo);

	MenuObject *multiplayer = new MenuObject(font->getText("Multiplayer", 100.0f), vec3(100, 250, 0)); 
	scene.add(multiplayer); 
	menuMultiplayer->add(multiplayer);
	
	MenuObject *hosted = new MenuObject(font->getText("Play hosted", 50.0f), vec3(100, 450, 0)); 
	scene.add(hosted); 
	menuMultiplayer->add(hosted);
	hosted->OnActivate = new SetMenuAction(this, menuConnecting);

	MenuObject *JoinLanGame = new MenuObject(font->getText("Join LAN Game", 50.0f), vec3(100, 525, 0)); 
	scene.add(JoinLanGame); 
	menuMultiplayer->add(JoinLanGame);
	JoinLanGame->OnActivate = new SetMenuAction(this, menuJoinGame);

	MenuObject *hostLanGame = new MenuObject(font->getText("Host LAN Game", 50.0f), vec3(100, 600, 0)); 
	scene.add(hostLanGame); 
	menuMultiplayer->add(hostLanGame);
	hostLanGame->OnActivate = new SetMenuAction(this, menuHostGame);

	menuMultiplayer->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createExit() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuExit->add(mainLogo);

	MenuObject *exit = new MenuObject(font->getText("Exit", 100.0f), vec3(100, 250, 0)); 
	scene.add(exit); 
	menuExit->add(exit);
	
	MenuObject *areyousure = new MenuObject(font->getText("Are you sure you want to quit?", 60.0f), vec3(100, 400, 0)); 
	scene.add(areyousure); 
	menuExit->add(areyousure);

	MenuObject *yes = new MenuObject(font->getText("Yes", 50.0f), vec3(200, 575, 0)); 
	scene.add(yes); 
	menuExit->add(yes);
	yes->OnActivate = new ArbitraryAction(this, &Menu::QuitGame);

	MenuObject *no = new MenuObject(font->getText("No", 50.0f), vec3(700, 575, 0)); 
	scene.add(no); 
	menuExit->add(no);
	no->OnActivate = new SetMenuAction(this, menuMainMenu);

	menuExit->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createLoadSave() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuLoadSave->add(mainLogo);

	MenuObject *singlePlayer = new MenuObject(font->getText("Load game", 100.0f), vec3(100, 250, 0)); 
	scene.add(singlePlayer); 
	menuLoadSave->add(singlePlayer);
	
	menuLoadSave->OnCancel = new SetMenuAction(this, menuSinglePlayer);
}

void Menu::createAudioSettings() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuAudioSettings->add(mainLogo);

	MenuObject *audioSettings = new MenuObject(font->getText("Audio settings", 100.0f), vec3(100, 250, 0)); 
	scene.add(audioSettings); 
	menuAudioSettings->add(audioSettings);
	
	MenuObject *masterVolume = new MenuObject(font->getText("Master volume", 25.0f), vec3(100, 450, 0)); 
	scene.add(masterVolume); 
	menuAudioSettings->add(masterVolume);
	masterVolume->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *voiceVolume = new MenuObject(font->getText("Voice volume", 25.0f), vec3(100, 475, 0)); 
	scene.add(voiceVolume); 
	menuAudioSettings->add(voiceVolume);
	voiceVolume->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *effectVolume = new MenuObject(font->getText("Effect volume", 25.0f), vec3(100, 500, 0)); 
	scene.add(effectVolume); 
	menuAudioSettings->add(effectVolume);
	effectVolume->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *musicVolume = new MenuObject(font->getText("Music volume", 25.0f), vec3(100, 525, 0)); 
	scene.add(musicVolume); 
	menuAudioSettings->add(musicVolume);
	musicVolume->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *videoSettings = new MenuObject(font->getText("Video settings", 50.0f), vec3(100, 600, 0)); 
	scene.add(videoSettings); 
	menuAudioSettings->add(videoSettings);
	videoSettings->OnActivate = new SetMenuAction(this, menuVideoSettings);

	MenuObject *graphicsSettings = new MenuObject(font->getText("Graphics settings", 50.0f), vec3(100, 675, 0)); 
	scene.add(graphicsSettings); 
	menuAudioSettings->add(graphicsSettings);
	graphicsSettings->OnActivate = new SetMenuAction(this, menuGraphicsSettings);

	menuAudioSettings->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createVideoSettings() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuVideoSettings->add(mainLogo);

	MenuObject *videoSettings = new MenuObject(font->getText("Video settings", 100.0f), vec3(100, 250, 0)); 
	scene.add(videoSettings); 
	menuVideoSettings->add(videoSettings);
	
	MenuObject *screenResolution = new MenuObject(font->getText("Screen resolution", 25.0f), vec3(100, 450, 0)); 
	scene.add(screenResolution); 
	menuVideoSettings->add(screenResolution);
	screenResolution->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *screenAspectRatio = new MenuObject(font->getText("Screen aspect ratio", 25.0f), vec3(100, 475, 0)); 
	scene.add(screenAspectRatio); 
	menuVideoSettings->add(screenAspectRatio);
	screenAspectRatio->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *gammaCorrection = new MenuObject(font->getText("Gamma correction", 25.0f), vec3(100, 500, 0)); 
	scene.add(gammaCorrection); 
	menuVideoSettings->add(gammaCorrection);
	gammaCorrection->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *gamma = new MenuObject(font->getText("Gamma", 25.0f), vec3(100, 525, 0)); 
	scene.add(gamma); 
	menuVideoSettings->add(gamma);
	gamma->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *audioSettings = new MenuObject(font->getText("Audio settings", 50.0f), vec3(100, 600, 0)); 
	scene.add(audioSettings); 
	menuVideoSettings->add(audioSettings);
	audioSettings->OnActivate = new SetMenuAction(this, menuAudioSettings);

	MenuObject *graphicsSettings = new MenuObject(font->getText("Graphics settings", 50.0f), vec3(100, 675, 0)); 
	scene.add(graphicsSettings); 
	menuVideoSettings->add(graphicsSettings);
	graphicsSettings->OnActivate = new SetMenuAction(this, menuGraphicsSettings);

	menuVideoSettings->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createGraphicsSettings() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuGraphicsSettings->add(mainLogo);

	MenuObject *graphicsSettings = new MenuObject(font->getText("Graphics settings", 100.0f), vec3(100, 250, 0)); 
	scene.add(graphicsSettings); 
	menuGraphicsSettings->add(graphicsSettings);
	
	MenuObject *shaderQualityLevel = new MenuObject(font->getText("Shader quality level", 25.0f), vec3(100, 450, 0)); 
	scene.add(shaderQualityLevel); 
	menuGraphicsSettings->add(shaderQualityLevel);
	shaderQualityLevel->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *audioSettings = new MenuObject(font->getText("Audio settings", 50.0f), vec3(100, 600, 0)); 
	scene.add(audioSettings); 
	menuGraphicsSettings->add(audioSettings);
	audioSettings->OnActivate = new SetMenuAction(this, menuAudioSettings);

	MenuObject *videoSettings = new MenuObject(font->getText("Video settings", 50.0f), vec3(100, 675, 0)); 
	scene.add(videoSettings); 
	menuGraphicsSettings->add(videoSettings);
	videoSettings->OnActivate = new SetMenuAction(this, menuVideoSettings);

	menuGraphicsSettings->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createCredits() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 100.0f), vec3(200, 250, 0)); 
	scene.add(mainLogo); 
	menuCredits->add(mainLogo);

	MenuObject *programming = new MenuObject(font->getText("Programming", 50.0f), vec3(100, 350, 0)); 
	scene.add(programming); 
	menuCredits->add(programming);
	MenuObject *peterBindels = new MenuObject(font->getText("Peter Bindels", 30.0f), vec3(100, 400, 0)); 
	scene.add(peterBindels); 
	menuCredits->add(peterBindels);

	MenuObject *threeDModeling = new MenuObject(font->getText("3D Modeling", 50.0f), vec3(100, 450, 0)); 
	scene.add(threeDModeling); 
	menuCredits->add(threeDModeling);

	MenuObject *moContinue = new MenuObject(font->getText("Somebody that I used to know", 30.0f), vec3(100, 500, 0)); 
	scene.add(moContinue); 
	menuCredits->add(moContinue);

	menuCredits->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createJoinGame() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuJoinGame->add(mainLogo);

	MenuObject *joinGame = new MenuObject(font->getText("Join game", 100.0f), vec3(100, 250, 0)); 
	scene.add(joinGame); 
	menuJoinGame->add(joinGame);
	
	MenuObject *enterServerName = new MenuObject(font->getText("Enter server name", 50.0f), vec3(100, 450, 0)); 
	scene.add(enterServerName); 
	menuJoinGame->add(enterServerName);
	enterServerName->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *join = new MenuObject(font->getText("Join", 50.0f), vec3(100, 600, 0)); 
	scene.add(join); 
	menuJoinGame->add(join);
	join->OnActivate = new SetMenuAction(this, menuConnecting);

	menuJoinGame->OnCancel = new SetMenuAction(this, menuMultiplayer);
}

void Menu::createHostGame() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuHostGame->add(mainLogo);

	MenuObject *singlePlayer = new MenuObject(font->getText("Create game", 100.0f), vec3(100, 250, 0)); 
	scene.add(singlePlayer); 
	menuHostGame->add(singlePlayer);
	
	menuHostGame->OnCancel = new SetMenuAction(this, menuMultiplayer);
}

void Menu::createConnecting() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuConnecting->add(mainLogo);

	MenuObject *connecting = new MenuObject(font->getText("Connecting", 100.0f), vec3(200, 450, 0)); 
	scene.add(connecting); 
	menuConnecting->add(connecting);

	menuConnecting->OnCancel = new SetMenuAction(this, menuJoinGame);
}

void Menu::createHostedGame() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuHostedGame->add(mainLogo);

	MenuObject *hostedGame = new MenuObject(font->getText("Hosted game", 100.0f), vec3(100, 250, 0)); 
	scene.add(hostedGame); 
	menuHostedGame->add(hostedGame);
	
	menuHostedGame->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createJoinedGame() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuJoinedGame->add(mainLogo);

	MenuObject *joinedGame = new MenuObject(font->getText("Joined game", 100.0f), vec3(100, 250, 0)); 
	scene.add(joinedGame); 
	menuJoinedGame->add(joinedGame);
	
	menuJoinedGame->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createGameStarting() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuGameStarting->add(mainLogo);

	MenuObject *starting = new MenuObject(font->getText("Starting", 100.0f), vec3(200, 450, 0)); 
	scene.add(starting); 
	menuGameStarting->add(starting);

	menuGameStarting->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createConnectionLost() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuConnectionLost->add(mainLogo);

	MenuObject *connectionLost = new MenuObject(font->getText("Connection Lost", 100.0f), vec3(200, 450, 0)); 
	scene.add(connectionLost); 
	menuConnectionLost->add(connectionLost);

	menuConnectionLost->OnCancel = new SetMenuAction(this, menuMainMenu);
}

void Menu::createIngame() {
	char logo[2] = {0}; logo[0] = 31;
	MenuObject *mainLogo = new MenuObject(font->getText(logo, 20.0f), vec3(800, 250, 0)); 
	scene.add(mainLogo); 
	menuIngame->add(mainLogo);

	MenuObject *ingame = new MenuObject(font->getText("In-game", 100.0f), vec3(100, 250, 0)); 
	scene.add(ingame); 
	menuIngame->add(ingame);
	
	MenuObject *endGame = new MenuObject(font->getText("End game", 50.0f), vec3(100, 525, 0)); 
	scene.add(endGame); 
	menuIngame->add(endGame);
	endGame->OnActivate = new SetMenuAction(this, menuMainMenu);

	MenuObject *audioSettings = new MenuObject(font->getText("Audio settings", 50.0f), vec3(100, 525, 0)); 
	scene.add(audioSettings); 
	menuIngame->add(audioSettings);
	audioSettings->OnActivate = new SetMenuAction(this, menuAudioSettings);

	MenuObject *videoSettings = new MenuObject(font->getText("Video settings", 50.0f), vec3(100, 600, 0)); 
	scene.add(videoSettings); 
	menuIngame->add(videoSettings);
	videoSettings->OnActivate = new SetMenuAction(this, menuVideoSettings);

	MenuObject *graphicsSettings = new MenuObject(font->getText("Graphics settings", 50.0f), vec3(100, 675, 0)); 
	scene.add(graphicsSettings); 
	menuIngame->add(graphicsSettings);
	graphicsSettings->OnActivate = new SetMenuAction(this, menuGraphicsSettings);

	menuIngame->OnCancel = new SetMenuAction(this, menuMainMenu);
}
