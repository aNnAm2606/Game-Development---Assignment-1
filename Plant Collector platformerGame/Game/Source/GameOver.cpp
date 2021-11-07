#include "GameOver.h"

#include "App.h"
#include "Animation.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"

GameOver::GameOver(bool startEnabled) : Module(startEnabled)
{
	name.Create("gameOver");

	sleep.PushBack({ 96, 64, 32, 32 });
	sleep.PushBack({ 0, 96, 32, 32 });
	sleep.PushBack({ 32, 96, 32, 32 });
	sleep.PushBack({ 64, 96, 32, 32 });
	sleep.PushBack({ 96, 96, 32, 32 });
	sleep.PushBack({ 0, 128, 32, 32 });
	sleep.PushBack({ 32, 128, 32, 32 });
	sleep.PushBack({ 64, 128, 32, 32 });
	sleep.PushBack({ 96, 128, 32, 32 });
	sleep.loop = false;
	sleep.speed = 0.08f;
}

GameOver::~GameOver()
{
}

// Called before render is available
bool GameOver::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Game Over");
	bool ret = true;

	gameOverTitle.Create(config.child("gameOverTitle").child_value());
	background.Create(config.child("background").child_value());
	restartButton.Create(config.child("spaceToRestart").child_value());
	characterFile.Create(config.child("spritesFolder").child_value());

	return ret;
}

// Load assets
bool GameOver::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	bg1 = app->tex->Load(background.GetString());
	title = app->tex->Load(gameOverTitle.GetString());
	button = app->tex->Load(restartButton.GetString());
	character = app->tex->Load(characterFile.GetString());

	LOG("setting camrea to (0,0)");
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// stating animation
	currentAnimation = &sleep;

	return ret;
}

bool GameOver::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->sceneIntro, 120);
	}

	currentAnimation->Update();
	
	return ret;
}

bool GameOver::PostUpdate()
{
	frames++;
	app->render->DrawTexture(bg1, 0, 0, NULL);
	app->render->DrawTexture(title, 0, 0, NULL);

	if ((frames / 60) % 2 == 0)
	{
		app->render->DrawTexture(button, 0, 0, NULL);
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(character, 463, 220, &rect);

	return true;
}

bool GameOver::CleanUp()
{
	LOG("Freeing scene GameOver");
	app->tex->UnLoad(bg1);
	app->tex->UnLoad(title);
	app->tex->UnLoad(button);
	app->tex->UnLoad(character);
	return true;
}