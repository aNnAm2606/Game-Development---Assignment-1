#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Logo.h"
#include "SceneIntro.h"
#include "Level1.h"
#include "Lives.h"
#include "GameOver.h"
#include "WinScreen.h"
#include "Map.h"
#include "Physics.h"
#include "Enemy.h"
#include "FadeToBlack.h"
#include "Particles.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

#include "External/Optick/include/optick.h"

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	fade = new FadeToBlack(true);
	logo = new Logo(false);
	sceneIntro = new SceneIntro(true);
	level1 = new Level1(false);
	lives = new Lives(false);
	winScreen = new WinScreen(false);
	gameOver = new GameOver(false);
	map = new Map(true);
	particles = new Particles(false);
	physics = new Physics(true);
	entityManager = new EntityManager(false);
	guiManager = new GuiManager(true);
	fonts = new Fonts(false);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(physics);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(particles);
	//AddModule(enemy);
	AddModule(logo);
	AddModule(sceneIntro);
	AddModule(entityManager);
	AddModule(level1);
	AddModule(lives);
	AddModule(gameOver);
	AddModule(winScreen);
	AddModule(guiManager);
	AddModule(fade);
	AddModule(fonts);

	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		title.Create(config.child("App").child("title").child_value()); // L01: TODO 4: Read the title from the config file
		win->SetTitle(title.GetString());
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node;
			node = config.child(item->data->name.GetString());
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}

	deltaTime = 0.0f;
	lastTime = 0;

	msFrame = 1.0f / FPS;

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	/*OPTICK_CATEGORY("UpdateLogic", Optick::Category::GameLogic);*/

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	/*OPTICK_CATEGORY("Prepare UpdateLogic", Optick::Category::GameLogic);*/
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: TODO 1: This is a good place to call Load / Save methods
	if (loadGameRequest == true) LoadFromFile();
	if (saveGameRequest == true) SaveToFile();
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	/*OPTICK_CATEGORY("PreUpdate Logic", Optick::Category::GameLogic)*/

	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if (item->data->IsEnabled())
			ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	/*OPTICK_CATEGORY("DoUpdate Logic", Optick::Category::GameLogic)*/

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if (item->data->IsEnabled())
			ret = item->data->Update(deltaTime);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	/*OPTICK_CATEGORY("PostUpdate Logic", Optick::Category::GameLogic)*/

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if (item->data->IsEnabled())
			ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequest = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequest = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file(SAVE_STATE_FILENAME);

	if (result == NULL)
	{
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
		LOG("File Loaded succesfuly");
	}

	loadGameRequest = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveToFile() const
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file(SAVE_STATE_FILENAME);

	//send to each module the XML element SaveState()
	//check for errors
	if (result == NULL)
	{
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		//if ok
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->SaveState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
		LOG("File Saved succesfuly");
	}

	//function to asve xml to file (override)
	gameStateFile.save_file(SAVE_STATE_FILENAME);

	saveGameRequest = false;

	return ret;
}


