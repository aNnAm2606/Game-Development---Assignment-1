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
#include "EntityManager.h"
#include "Particles.h"
#include "FadeToBlack.h"
#include "Enemy.h"
#include "Lives.h"

#include "Defs.h"
#include "Log.h"

#include "External/Optick/include/optick.h"

Level1::Level1(bool startEnabled) : Module(startEnabled)
{
	name.Create("Level1");

	//Chest Animation
	chestClosed.PushBack({ 0, 0, 32, 32 });
	chestClosed.loop = false;

	chestOpened.PushBack({ 32, 0, 32, 32 });
	chestOpened.PushBack({ 64, 0, 32, 32 });
	chestOpened.PushBack({ 96, 0, 32, 32 });
	chestOpened.loop = false;
	chestOpened.speed = 0.05f;

	//CheckPoint Animation
	flag0.PushBack({ 0, 0, 48, 48 });
	flag0.loop = false;

	flag1.PushBack({   0, 0, 48, 48 });
	flag1.PushBack({  48, 0, 48, 48 });
	flag1.PushBack({  96, 0, 48, 48 });
	flag1.PushBack({ 144, 0, 48, 48 });
	flag1.loop = true;
	flag1.speed = 0.05f;
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
	textureFlag.Create(config.child("textureFlag").child_value());

	return ret;
}

// Called before the first frame
bool Level1::Start()
{
	app->entityManager->Enable();

	Player* player = (Player*)app->entityManager->CreateEntity(PLAYER, iPoint{ 120, 1060 });
	app->entityManager->SetPlayer(player);

	app->map->Enable();
	// L03: DONE: Load map
	app->map->Load("test.tmx");
	
	// Load music
	//app->audio->PlayMusic(app->audio->soundtrack.GetString());
	checkPointSound = app->audio->LoadFx("Assets/audio/fx/checkPointSound.wav");

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
	flag = app->tex->Load(textureFlag.GetString());
	

	// stating animation
	currentChestAnimation = &chestClosed;
	currentFlagAnim = &flag0;

	app->particles->Enable();
	app->entityManager->CreateEntity(DOG, iPoint{ 400,1061 });
	app->entityManager->CreateEntity(CAT, iPoint{ 600,1061 });
	app->entityManager->CreateEntity(BIRD, iPoint{ 870,805 });

	//app->enemy->Enable();
	app->map->Colliders();
	app->lives->Enable();

	//Adding coins
	app->particles->CreateParticles(COINP, 672, 960);
	app->particles->CreateParticles(COINP, 608, 1024);
	app->particles->CreateParticles(COINP, 320, 1024);
	app->particles->CreateParticles(COINP, 832, 896);
	app->particles->CreateParticles(COINP, 1152, 1248);
	app->particles->CreateParticles(COINP, 352, 768);
	app->particles->CreateParticles(COINP, 512, 768);
	app->particles->CreateParticles(COINP, 672, 704);
	app->particles->CreateParticles(COINP, 832, 672);
	app->particles->CreateParticles(COINP, 992, 1376);
	app->particles->CreateParticles(COINP, 1120, 1376);
	app->particles->CreateParticles(COINP, 1280, 1376);
	app->particles->CreateParticles(COINP, 1408, 1312);
	app->particles->CreateParticles(COINP, 1568, 1312);
	app->particles->CreateParticles(COINP, 1728, 1312);
	app->particles->CreateParticles(COINP, 1792, 1280);
	app->particles->CreateParticles(COINP, 1856, 1280);
	app->particles->CreateParticles(COINP, 1920, 1248);
	app->particles->CreateParticles(COINP, 1984, 1248);

	//Adding hearts
	app->particles->CreateParticles(HEART, 1248, 928);

	// Hidden Key
	app->particles->CreateParticles(KEYP, 2112, 1248);

	// Reset
	dontPlayAudio = false;

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

	//if ((app->entityManager->player->chestFound == true || app->map->debugColliders == true) && app->entityManager->player->chestOpen == false) app->render->DrawTexture(app->entityManager->player->tutorialsTex, 2039, 1207, &app->entityManager->player->chestRect);

	// update animation
	currentChestAnimation->Update();
	currentFlagAnim->Update();
	
	return true;
}

// Called each loop iteration
bool Level1::PostUpdate()
{
	/*OPTICK_CATEGORY("lvl1 PostUpdate", Optick::Category::Scene);*/

	bool ret = true;

	ChestRect = currentChestAnimation->GetCurrentFrame();
	if (app->entityManager->player->chestOpen == true)
	{
		currentChestAnimation = &chestOpened;
		app->render->DrawTexture(treasureChest, 2048, 1248, &ChestRect);
	}
	else 
	{
		app->render->DrawTexture(treasureChest, 2048, 1248, &ChestRect);
	}

	flagRect = currentFlagAnim->GetCurrentFrame();
	if (app->entityManager->player->checkPointReached == true)
	{
		currentFlagAnim = &flag1;
		app->render->DrawTexture(flag, 1728, 1040, &flagRect);
		if (dontPlayAudio == false) app->audio->PlayFx(checkPointSound);
		dontPlayAudio = true;
	}
	else
	{
		app->render->DrawTexture(flag, 1728, 1040, &flagRect);
	}

	// Game WiN/Lose
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	if (app->entityManager->player->lives == 0 || app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->gameOver);
	}

	if (app->entityManager->player->win == true || app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->winScreen);
	}

	return ret;
}

// Called before quitting
bool Level1::CleanUp()
{
	LOG("Freeing scene");
	app->entityManager->CleanUp();
	app->entityManager->Disable();
	//app->enemy->Disable();
	app->particles->Disable();
	app->map->Disable();
	app->map->Unload();

	return true;
}
