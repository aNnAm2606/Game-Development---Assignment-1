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
	bgTexture = app->tex->Load("Assets/textures/test.png");

	return ret;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade_To_Black(this, (Module*)app->scene, 90);
	}

	return ret;
}

bool SceneIntro::PostUpdate()
{
	app->render->DrawTexture(bgTexture, 0, 0, NULL);
	
	return true;
}

bool SceneIntro::CleanUp()
{
	return true;
}