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

	background = app->tex->Load("Assets/assets/10_Sky.png");
	background1 = app->tex->Load("Assets/assets/09_Forest.png");
	background2 = app->tex->Load("Assets/assets/08_Forest.png");
	background3 = app->tex->Load("Assets/assets/07_Forest.png");
	background4 = app->tex->Load("Assets/assets/06_Forest.png");
	background5 = app->tex->Load("Assets/assets/05_Particles.png");
	background6 = app->tex->Load("Assets/assets/04_Forest.png");
	background7 = app->tex->Load("Assets/assets/03_Particles.png");
	background8 = app->tex->Load("Assets/assets/02_Bushes.png");
	background9 = app->tex->Load("Assets/assets/01_Mist.png");
	app->map->Load("tiles.tmx");
	
	// Load music
	//app->audio->PlayMusic("Assets/audio/music/Day_of_the_river.ogg");

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

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->render->DrawTexture(background, 0, 0, NULL, 0.3f);
	app->render->DrawTexture(background1, 0, 0, NULL, 0.2f);
	app->render->DrawTexture(background2, 0, 0, NULL, 0.3f);
	app->render->DrawTexture(background3, 0, 0, NULL, 0.2f);
	app->render->DrawTexture(background4, 0, 0, NULL, 0.3f);
	app->render->DrawTexture(background5, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(background6, 0, 0, NULL, 0.4f);
	app->render->DrawTexture(background7, 0, 0, NULL, 0.5f);
	app->render->DrawTexture(background8, 0, 0, NULL, 0.55f);
	app->render->DrawTexture(background9, 0, 0, NULL, 0.6f);
	app->map->Draw();

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
