#include "WinScreen.h"

#include "App.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"

WinScreen::WinScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("WinScreen");

	idle.PushBack({ 0, 64, 32, 32 });
	idle.PushBack({ 32, 64, 32, 32 });
	idle.PushBack({ 64, 64, 32, 32 });
	idle.PushBack({ 96, 64, 32, 32 });
	idle.loop = true;
	idle.speed = 0.08f;
}

WinScreen::~WinScreen()
{
}

// Called before render is available
bool WinScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Game Over");
	bool ret = true;

	WinTitle.Create(config.child("WinTitle").child_value());
	background.Create(config.child("background").child_value());
	restartButton.Create(config.child("spaceToRestart").child_value());
	characterFile.Create(config.child("spritesFolder").child_value());

	return ret;
}

// Load assets
bool WinScreen::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	bg1 = app->tex->Load(background.GetString());
	title = app->tex->Load(WinTitle.GetString());
	button = app->tex->Load(restartButton.GetString());
	character = app->tex->Load(characterFile.GetString());

	LOG("setting camrea to (0,0)");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// stating animation
	currentAnimation = &idle;

	return ret;
}

bool WinScreen::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->sceneIntro, 120);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->level1, 180);
	}

	currentAnimation->Update();
	
	return ret;
}

bool WinScreen::PostUpdate()
{
	frames++;
	app->render->DrawTexture(bg1, 0, 0, NULL);
	app->render->DrawTexture(title, 0, 0, NULL);

	if ((frames / 60) % 2 == 0)
	{
		app->render->DrawTexture(button, 0, 0, NULL);
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(character, 463, 190, &rect);

	return true;
}

bool WinScreen::CleanUp()
{
	LOG("Freeing scene GameOver");
	app->tex->UnLoad(bg1);
	app->tex->UnLoad(title);
	app->tex->UnLoad(button);
	app->tex->UnLoad(character);
	return true;
}