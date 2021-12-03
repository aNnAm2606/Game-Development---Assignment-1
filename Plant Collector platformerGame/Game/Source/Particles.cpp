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

#include <stdio.h>

Particles::Particles(bool startEnabled) : Module(startEnabled)
{
	name.Create("particles");
}

Particles::~Particles()
{

}

// Awake player
bool Particles::Awake(pugi::xml_node& config)
{
	LOG("Loading Particles");
	bool ret = true;

	return ret;
}

bool Particles::Start()
{
	LOG("Loading Particle textures");

	bool ret = true;

	return ret;
}

bool Particles::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Particles::PostUpdate()
{
	bool ret = true;

	return ret;
}

void Particles::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}

bool Particles::LoadState(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	playerBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	return true;
}

bool Particles::SaveState(pugi::xml_node& data) const
{
	data.child("position").attribute("x").set_value(position.x);
	data.child("position").attribute("y").set_value(position.y);
	return true;
}

bool Particles::CleanUp()
{
	LOG("Destroying Particles");
	bool ret = true;

	return ret;
}