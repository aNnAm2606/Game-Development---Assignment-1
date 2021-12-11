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
}

Particles::~Particles()
{

}

// Awake player
bool Particles::Awake(pugi::xml_node& config)
{
	LOG("Loading Particles");
	textureCoin.Create(config.child("textureCoin").child_value());
	bool ret = true;

	return ret;
}

bool Particles::Start()
{
	LOG("Loading Particle textures");

	// Load Items coins, chests, powerups
	coin = app->tex->Load(textureCoin.GetString());

	// stating animation
	currentCoinsAnim = &coins;

	bool ret = true;

	return ret;
}

bool Particles::Update(float dt)
{
	bool ret = true;

	//if ((coinCollision == true || app->map->debugColliders == true) && app->particles->coinCollected == false) app->render->DrawTexture(&app->particles->coinRect);
	// update animation
	currentCoinsAnim->Update();

	return ret;
}

bool Particles::PostUpdate()
{
	bool ret = true;

	return ret;
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