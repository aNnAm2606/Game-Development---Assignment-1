#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//img = app->tex->Load("Assets/textures/test.png");
	//app->map->Load("hello.tmx");
	app->map->Load("tiles.tmx");
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	// Load the volume audio from last game
	//app->LoadGameRequest();

	// Load the backgrounds
	sky = app->tex->Load("Assets/textures/10_Sky.png");
	forestBushes = app->tex->Load("Assets/textures/09_Forest.png");
	treesBack = app->tex->Load("Assets/textures/08_Forest.png");
	treesMid = app->tex->Load("Assets/textures/07_Forest.png");
	treesMidClose = app->tex->Load("Assets/textures/06_Forest.png");
	particles = app->tex->Load("Assets/textures/05_Particles.png");
	treesClose = app->tex->Load("Assets/textures/04_Forest.png");
	particles1 = app->tex->Load("Assets/textures/03_Particles.png");
	bushes = app->tex->Load("Assets/textures/02_Bushes.png");
	mist = app->tex->Load("Assets/textures/01_Mist.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	// L02: Homework: volume control
	//if the volume is not at max we can set it higher
	if (app->audio->volume < 128)
	{
		if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
			app->audio->volume+=1;
	}
	if (app->audio->volume > 0)
	{
		if (app->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT)
			app->audio->volume-=1;
	}


	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->map->debugColliders = !app->map->debugColliders;
	}

	// Draw background
	app->render->DrawTexture(sky, 0, 0, NULL, -0.001f);
	app->render->DrawTexture(forestBushes, 0, 0, NULL, 0.1f);
	app->render->DrawTexture(treesBack, 0, 0, NULL, 0.2f);
	app->render->DrawTexture(treesMid, 0, 0, NULL, 0.3f);
	app->render->DrawTexture(treesMidClose, 0, 0, NULL, 0.4f);
	app->render->DrawTexture(particles, 0, 0, NULL, 0.45f);
	app->render->DrawTexture(treesClose, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(particles1, 0, 0, NULL, 0.55f);
	app->render->DrawTexture(bushes, 0, 0, NULL, 0.6f);
	app->render->DrawTexture(mist, 0, 0, NULL, 0.62f);

	// Draw map
	app->map->Draw();
	app->map->DrawColliders();

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
