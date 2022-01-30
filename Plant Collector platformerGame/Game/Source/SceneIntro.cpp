#include "SceneIntro.h"

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
#include "Window.h"
#include "GuiManager.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneIntro");

	walk.PushBack({ 0, 0, 32, 32 });
	walk.PushBack({ 32, 0, 32, 32 });
	walk.PushBack({ 64, 0, 32, 32 });
	walk.PushBack({ 96, 0, 32, 32 });
	walk.loop = true;
	walk.speed = 0.2f;
}

SceneIntro::~SceneIntro()
{
}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	screenImage.Create(config.child("IntroScreen").child_value());
	StartButton.Create(config.child("spaceToStart").child_value());
	title.Create(config.child("title").child_value());
	playerSprites.Create(config.child("spritesFolder").child_value());
	settingsPannel.Create(config.child("settingsPannel").child_value());

	return ret;
}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	bg1 = app->tex->Load(screenImage.GetString());
	bg2 = app->tex->Load(title.GetString());
	settingsTexture = app->tex->Load(settingsPannel.GetString());

	character = app->tex->Load(playerSprites.GetString());

	// stating animation
	currentAnimation = &walk;
	cPos.x = 0;
	cPos.y = 260;
	
	LOG("settting camera to (0,0)");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//GUI
	uint x;
	uint y;
	app->win->GetWindowSize(x, y);
	startButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "start", { ((int)x / 2)-55, (int)y / 2, 111, 52 }, this);
	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "continue", { ((int)x / 2)-55, (int)y-200, 111, 52 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "settings", { 30, 20, 44, 44 }, this);

	return ret;
}

bool SceneIntro::LoadState(pugi::xml_node& data)
{
	level1 = data.child("Level").attribute("level1").as_bool();
	level2 = data.child("Level").attribute("level1").as_bool();
	
	return true;
}

bool SceneIntro::SaveState(pugi::xml_node& data) const
{
	if (app->level1->isEnabled==true && app->level1->F6Load == true)
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

bool SceneIntro::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	currentAnimation->Update();

	cPos.x += 5;
	
	return ret;
}

bool SceneIntro::PostUpdate()
{
	frames++;
	app->render->DrawTexture(bg1, 0, 0, NULL);
	app->render->DrawTexture(bg2, 0, 0, NULL);

	// commented the press space to start and replaced by GUI button
	//if ((frames / 60) % 2 == 0)
	//{
	//	app->render->DrawTexture(bg3, 0, 0, NULL);
	//}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(character, cPos.x, cPos.y, &rect);

	//Draw GUI
	app->guiManager->Draw();

	if (showPannel == true)
	{
		app->render->DrawTexture(settingsTexture, 150, 80, NULL);
	}

	return true;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (showPannel == false)
		{
			if (control->id == 1)
			{
				LOG("Click on start button");
				app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
			}
			if (control->id == 2)
			{
				LOG("Click on continue button");
			}
		}
		if (control->id == 3)
		{
			LOG("Click on settings button");
			showPannel = !showPannel;
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	LOG("Freeing scene Intro");
	app->tex->UnLoad(bg1);
	app->tex->UnLoad(bg2);
	app->tex->UnLoad(bg3);
	app->tex->UnLoad(character);
	return true;
}