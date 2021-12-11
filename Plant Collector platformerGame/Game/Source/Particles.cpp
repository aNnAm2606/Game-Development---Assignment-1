#include "Particles.h"

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

#include <stdio.h>

Particles::Particles(bool startEnabled) : Module(startEnabled)
{
	name.Create("particles");

	coins.PushBack({ 0, 0, 10, 10 });
	coins.PushBack({ 10, 0, 10, 10 });
	coins.PushBack({ 20, 0, 10, 10 });
	coins.PushBack({ 30, 0, 10, 10 });
	coins.loop = true;
	coins.speed = 0.1f;

	hearts.PushBack({ 0, 0, 10, 10 });
	hearts.PushBack({ 10, 0, 10, 10 });
	hearts.loop = true;
	hearts.speed = 0.1f;
}

Particles::~Particles()
{

}

// Awake player
bool Particles::Awake(pugi::xml_node& config)
{
	LOG("Loading Particles");
	textureCoin.Create(config.child("textureCoin").child_value());
	textureHeart.Create(config.child("textureHeart").child_value());

	return true;
}

bool Particles::Start()
{
	LOG("Loading Particle textures");

	// Load Items coins, chests, powerups
	coin = app->tex->Load(textureCoin.GetString());
	heart = app->tex->Load(textureHeart.GetString());

	// stating animation
	currentCoinsAnim = &coins;
	currentHeartsAnim = &hearts;


	return true;
}

bool Particles::Update(float dt)
{


	//if ((coinCollision == true || app->map->debugColliders == true) && app->particles->coinCollected == false) app->render->DrawTexture(&app->particles->coinRect);
	// update animation
	currentCoinsAnim->Update();
	currentHeartsAnim->Update();

	return true;
}

bool Particles::PostUpdate()
{


	coinRect = currentCoinsAnim->GetCurrentFrame();
	if (app->particles->coinCollision == true)
	{
		/*currentCoinsAnim = &noCoin;*/
		app->render->DrawTexture(coin, 672, 960, &coinRect);
	}
	else
	{
		app->render->DrawTexture(coin, 672, 960, &coinRect);
	}

	heartRect = currentHeartsAnim->GetCurrentFrame();
	//if (app->particles->coinCollision == true)
	//{
	//	/*currentCoinsAnim = &noCoin;*/
	//	app->render->DrawTexture(heart, 288, 992, &heartRect);
	//}
	
		app->render->DrawTexture(heart, 288, 992, &heartRect);

	return true;
}

void Particles::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::COINS)
	{
		LOG("Coin collected");
		coinCollision = true;
	}
	else coinCollision = false;

}

bool Particles::LoadState(pugi::xml_node& data)
{
	return true;
}

bool Particles::SaveState(pugi::xml_node& data) const
{
	return true;
}

bool Particles::CleanUp()
{
	LOG("Destroying Particles");
	bool ret = true;

	return ret;
}