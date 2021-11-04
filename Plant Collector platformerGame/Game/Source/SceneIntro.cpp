#include "SceneIntro.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
}

SceneIntro::~SceneIntro()
{
}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	bg1 = app->tex->Load("Assets/textures/1.png");
	bg2 = app->tex->Load("Assets/textures/2.png");
	bg3 = app->tex->Load("Assets/textures/3.png");
	bg4 = app->tex->Load("Assets/textures/4.png");
	bg5 = app->tex->Load("Assets/textures/5.png");

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->scene);
	}

	return ret;
}

bool SceneIntro::PostUpdate()
{
	app->render->DrawTexture(bg1, 0, 0, NULL);
	app->render->DrawTexture(bg2, 0, 0, NULL);
	app->render->DrawTexture(bg3, 0, 0, NULL);
	app->render->DrawTexture(bg4, 0, 0, NULL);
	app->render->DrawTexture(bg5, 0, 0, NULL);
	
	return true;
}

bool SceneIntro::CleanUp()
{
	return true;
}