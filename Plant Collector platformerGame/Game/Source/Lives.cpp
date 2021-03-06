#include "Lives.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "Module.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "Player.h"
#include "EntityManager.h"

#include <stdio.h>


Lives::Lives(bool startEnabled) : Module(startEnabled)
{
	name.Create("Lives");
}

Lives::~Lives()
{
}

bool Lives::Awake(pugi::xml_node& config)
{
	textureHeart.Create(config.child("textureHeart").child_value());
	return true;
}

bool Lives::Start()
{
	hearts3 = app->tex->Load("Assets/textures/hearts3.png");
	hearts2 = app->tex->Load("Assets/textures/hearts2.png");
	hearts1 = app->tex->Load("Assets/textures/hearts1.png");

	return true;
}

bool Lives::PreUpdate()
{

	return true;
}

bool Lives::Update(float dt)
{
	// Draw lives
	if (app->entityManager->player->lives == 3)
	{
		app->render->DrawTexture(hearts3, 0, 0, NULL, true);
	}
	else if (app->entityManager->player->lives == 2)
	{
		app->render->DrawTexture(hearts2, 0, 0, NULL, true);
	}
	if (app->entityManager->player->lives == 1)
	{
		app->render->DrawTexture(hearts1, 0, 0, NULL, true);
	}

	return true;
}

bool Lives::PostUpdate()
{

	return true;
}


bool Lives::LoadState(pugi::xml_node& data)
{
	return true;
}

bool Lives::SaveState(pugi::xml_node& data) const
{
	return true;
}

bool Lives::CleanUp()
{
	LOG("Destroying Particles");
	bool ret = true;
	app->tex->UnLoad(hearts1);
	app->tex->UnLoad(hearts2);
	app->tex->UnLoad(hearts3);
	return ret;
}