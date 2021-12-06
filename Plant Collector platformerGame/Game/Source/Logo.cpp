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

	exist.PushBack({ 0, 0, 16, 16 });
	exist.PushBack({ 0, 0, 16, 16 });
	exist.loop = true;
	exist.speed = 0.2f;

}

Logo::~Logo()
{
}

// Called before render is available
bool Logo::Awake(pugi::xml_node& config)
{
	LOG("Loading Logo");
	bool ret = true;

	screenImage.Create(config.child("Logo").child_value());
	StartButton.Create(config.child("spaceToStart").child_value());

	return ret;
}

// Load assets
bool Logo::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	background = app->tex->Load(screenImage.GetString());
	icon = app->tex->Load(logoIcon.GetString());

	// stating animation
	cPos.x = 0;
	cPos.y = 260;

	LOG("settting camera to (0,0)");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool Logo::LoadState(pugi::xml_node& data)
{
	level1 = data.child("Level").attribute("level1").as_bool();
	level2 = data.child("Level").attribute("level1").as_bool();

	return true;
}

bool Logo::SaveState(pugi::xml_node& data) const
{
	if (app->level1->isEnabled == true && app->level1->F6Load == true)
	{
		data.child("Level").attribute("level1").set_value(true);
		LOG("level1 Save is TRUE");
	}
	else
	{
		data.child("Level").attribute("level1").set_value(false);
		LOG("level1 Save is FALSE");
	}

	//if (app->level2->isEnabled == true && app->level2->F6Load == true)
	//{
	//	data.child("Level").attribute("level2").set_value(true);
	//	LOG("level2 Save is TRUE");
	//}
	//else
	//{
	//	data.child("Level").attribute("level2").set_value(false);
	//	LOG("level2 Save is FALSE");
	//}

	return true;
}

bool Logo::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	//if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	//{
	//	if (level1 == true)
	//	{
	//		app->level1->F6Load = true;
	//		app->level2->F6Load = false;
	//		app->fade->Fade_To_Black(this, (Module*)app->level1);
	//	}
	//	else if (level2 == true)
	//	{
	//		app->level2->F6Load = true;
	//		app->level1->F6Load = false;
	//		app->fade->Fade_To_Black(this, (Module*)app->level2);
	//	}
	//}
	currentAnimation->Update();

	cPos.x += 5;

	return ret;
}

bool Logo::PostUpdate()
{
	frames++;
	app->render->DrawTexture(background, 0, 0, NULL);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(icon, cPos.x, cPos.y, &rect);

	return true;
}

bool Logo::CleanUp()
{
	LOG("Freeing Logo");
	app->tex->UnLoad(background);
	app->tex->UnLoad(icon);
	return true;
}