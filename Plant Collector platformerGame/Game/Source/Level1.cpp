#include "App.h"
#include "Animation.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Level1.h"
#include "Level2.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

#include "External/Optick/include/optick.h"

Level1::Level1(bool startEnabled) : Module(startEnabled)
{
	name.Create("Level1");

	coins.PushBack({ 0, 0, 10, 10 });
	coins.PushBack({ 10, 0, 10, 10 });
	coins.PushBack({ 20, 0, 10, 10 });
	coins.PushBack({ 30, 0, 10, 10 });
	coins.loop = true;
	coins.speed = 0.1f;

	dogs.PushBack({ 0, 0, 64, 64 });
	dogs.PushBack({ 64, 0, 64, 64 });
	dogs.PushBack({ 128, 0, 64, 64 });
	dogs.PushBack({ 192, 0, 64, 64 });
	dogs.loop = true;
	dogs.speed = 0.1f;


	chestClosed.PushBack({ 0, 0, 32, 32 });
	chestClosed.loop = false;

	chestOpened.PushBack({ 32, 0, 32, 32 });
	chestOpened.PushBack({ 64, 0, 32, 32 });
	chestOpened.PushBack({ 96, 0, 32, 32 });
	chestOpened.loop = false;
	chestOpened.speed = 0.05f;
}

// Destructor
Level1::~Level1()
{}

// Called before render is available
bool Level1::Awake(pugi::xml_node& config)
{
	LOG("Loading Level 1");
	bool ret = true;
	textureChest.Create(config.child("textureChest").child_value());
	textureCoin.Create(config.child("textureCoin").child_value());
	textureDog.Create(config.child("textureDog").child_value());

	return ret;
}

// Called before the first frame
bool Level1::Start()
{
	app->map->Enable();
	// L03: DONE: Load map
	app->map->Load("test.tmx");
	
	// Load music
	app->audio->PlayMusic(app->audio->soundtrack.GetString());

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


	// Load Items coins, chests, powerups
	treasureChest = app->tex->Load(textureChest.GetString());
	coin = app->tex->Load(textureCoin.GetString());
	dog = app->tex->Load(textureDog.GetString());

	// stating animation
	currentChestAnimation = &chestClosed;
	currentCoinsAnim = &coins;
	currentDogAnim = &dogs;

	app->player->Enable();
	app->map->Colliders();
	return true;
}

// Called each loop iteration
bool Level1::PreUpdate()
{
	/*OPTICK_CATEGORY("lvl1 PreUpdate", Optick::Category::Scene);*/
	return true;
}

// Called each loop iteration
bool Level1::Update(float dt)
{
	/*OPTICK_CATEGORY("lvl1 Update", Optick::Category::Scene);*/
    // L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		//if (app->level2->F6Load == true)
		//{
		//	F6Load = false;
		//	app->fade->Fade_To_Black(this, (Module*)app->level2);
		//}
	}
	
	if (app->map->debugColliders == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveGameRequest();
			F6Load = true;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		app->physics->CreateRectangle(250, 100, 32, 32, 0);

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
	app->render->DrawTexture(mist, 0, 0, NULL, 0.1f);

	// Draw map
	app->map->Draw();
	app->map->DrawColliders();

	if (app->player->controlsVisible == true || app->map->debugColliders == true) app->render->DrawTexture(app->player->controlsTex, 124, 930, NULL);
	if (app->player->tutorialVisible == true || app->map->debugColliders == true) app->render->DrawTexture(app->player->tutorialsTex, 462, 930, &app->player->tutRect);
	if ((app->player->chestFound     == true || app->map->debugColliders == true) && app->player->chestOpen == false) app->render->DrawTexture(app->player->tutorialsTex, 2039, 1207, &app->player->chestRect);
	if (app->player->tutorialVisible == true || app->map->debugColliders == true) app->render->DrawTexture(app->player->tutorialsTex, 1073, 1144, &app->player->ladderRect);
	if (app->player->tutorialVisible == true || app->map->debugColliders == true) app->render->DrawTexture(app->player->tutorialsTex, 2008, 958, &app->player->ladderRect);

	
	

	// update animation
	currentChestAnimation->Update();
	currentCoinsAnim->Update();

	return true;
}

// Called each loop iteration
bool Level1::PostUpdate()
{
	/*OPTICK_CATEGORY("lvl1 PostUpdate", Optick::Category::Scene);*/

	bool ret = true;

	SDL_Rect rect = currentChestAnimation->GetCurrentFrame();
	if (app->player->chestOpen == true)
	{
		currentChestAnimation = &chestOpened;
		app->render->DrawTexture(treasureChest, 2048, 1248, &rect);
	}
	else 
	{
		app->render->DrawTexture(treasureChest, 2048, 1248, &rect);
	}

	SDL_Rect rectC = currentCoinsAnim ->GetCurrentFrame();
	app->render->DrawTexture(coin, 992, 864, &rectC);

	SDL_Rect rectD = currentDogAnim->GetCurrentFrame();
	app->render->DrawTexture(dog, 1152, 895, &rectD);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->player->lives == 0 || app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->gameOver);
	}

	if (app->player->win == true || app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->winScreen);
	}

	return ret;
}

// Called before quitting
bool Level1::CleanUp()
{
	LOG("Freeing scene");

	app->player->Disable();
	app->map->Disable();
	app->map->Unload();

	return true;
}
