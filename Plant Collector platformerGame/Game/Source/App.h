#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "savegame.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class SceneIntro;
class Level1;
class WinScreen;
class GameOver;
class Map;
class Physics;
class Player;
class FadeToBlack;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadFromFile();
	bool SaveToFile() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	SceneIntro* sceneIntro;
	Level1* level1;
	WinScreen* winScreen;
	GameOver* gameOver;
	Map* map;
	Physics* physics;
	Player* player;
	FadeToBlack* fade;
	SString title;
	SString organization;

	float deltaTime;

private:

	int argc;
	char** args;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	pugi::xml_document configFile;
	pugi::xml_document gameStateFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;
	float dt;

	mutable bool saveGameRequest;
	bool loadGameRequest;

	unsigned int
		lastTime,
		currentTime;
	float msFrame;
};

extern App* app;

#endif	// __APP_H__