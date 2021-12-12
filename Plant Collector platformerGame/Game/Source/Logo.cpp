#include "Logo.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include "Player.h"
#include "Level1.h"
#include "Level2.h"

Logo::Logo(bool startEnabled) : Module(startEnabled)
{
	name.Create("logo");
}

Logo::~Logo()
{
}

// Called before render is available
bool Logo::Awake(pugi::xml_node& config)
{
	LOG("Loading Logo");
	bool ret = true;

	screenImage.Create(config.child("background").child_value());
	StartButton.Create(config.child("spaceToStart").child_value());
	logoIcon.Create(config.child("icon").child_value());

	return ret;
}

// Load assets
bool Logo::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	background = app->tex->Load(screenImage.GetString());
	space = app->tex->Load(StartButton.GetString());
	icon = app->tex->Load(logoIcon.GetString());

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}


bool Logo::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	app->fade->Fade_To_Black(this, (Module*)app->sceneIntro, 240);

	return ret;
}

bool Logo::PostUpdate()
{
	frames++;
	app->render->DrawTexture(background, 0, 0, NULL);
	//app->render->DrawTexture(space, 10, 0, NULL);
	app->render->DrawTexture(icon, 480, 270, NULL);

	return true;
}

bool Logo::CleanUp()
{
	LOG("Freeing Logo");
	app->tex->UnLoad(background);
	app->tex->UnLoad(space);
	app->tex->UnLoad(icon);
	return true;
}