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
#include "Player.h"

#include <stdio.h>

void Particles::CreateParticles(Type type, int x, int y)
{
	Particle* c = new Particle();
	c->type = type;
	c->position.x = x - 8;
	c->position.y = y - 8;
	c->body = app->physics->CreateRectangleSensor(x, y, 10, 10, 1);
	c->body->listener = this;
	if (type == COIN)
	{
		c->coins.PushBack({  0, 0, 10, 10 });
		c->coins.PushBack({ 10, 0, 10, 10 });
		c->coins.PushBack({ 20, 0, 10, 10 });
		c->coins.PushBack({ 30, 0, 10, 10 });
		c->coins.loop = true;
		c->coins.speed = 0.1f;
	}
	if (type == HEART)
	{
		c->hearts.PushBack({  0, 0, 10, 10 });
		c->hearts.PushBack({ 10, 0, 10, 10 });
		c->hearts.loop = true;
		c->hearts.speed = 0.05f;
	}
	if (type == KEY)
	{
		c->keys.PushBack({  0, 0, 8, 8 });
		c->keys.PushBack({ 12, 0, 8, 8 });
		c->keys.PushBack({ 24, 0, 8, 8 });
		c->keys.PushBack({ 36, 0, 8, 8 });
		c->keys.loop = true;
		c->keys.speed = 0.1f;
	}

	particles.add(c);
}

Particles::Particles(bool startEnabled) : Module(startEnabled)
{
	name.Create("Particles");
}

Particles::~Particles()
{
}

bool Particles::Awake(pugi::xml_node& config)
{
	textureCoin.Create(config.child("textureCoin").child_value());
	textureHeart.Create(config.child("textureHeart").child_value());
	textureKey.Create(config.child("textureKey").child_value());
	return true;
}

bool Particles::Start()
{
	coin = app->tex->Load(textureCoin.GetString());
	heart = app->tex->Load(textureHeart.GetString());
	key = app->tex->Load(textureKey.GetString());
	return true;
}

bool Particles::PreUpdate()
{
	for (ListItem<Particle*>* c = particles.start; c != NULL; c = c->next)
	{
		if (c->data->pendingToDelete)
		{
			app->physics->world->DestroyBody(c->data->body->body);
			particles.del(c);
			break;
		}
	}
	return true;
}

bool Particles::Update(float dt)
{
	for (ListItem<Particle*>* c = particles.start; c != NULL; c = c->next)
	{
		c->data->coins.Update();
		c->data->hearts.Update();
		c->data->keys.Update();
	}
	return true;
}

bool Particles::PostUpdate()
{
	for (ListItem<Particle*>* c = particles.start; c != NULL; c = c->next)
	{
		if (c->data->type == COIN) app->render->DrawTexture(coin, c->data->position.x + 4, c->data->position.y + 2, &c->data->coins.GetCurrentFrame());
		
		if (c->data->type == HEART) app->render->DrawTexture(heart, c->data->position.x + 4, c->data->position.y + 2, &c->data->hearts.GetCurrentFrame());
		
		if (c->data->type == KEY) app->render->DrawTexture(key, c->data->position.x + 4, c->data->position.y + 2, &c->data->keys.GetCurrentFrame());
		
	}
	return true;
}

void Particles::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	for (ListItem<Particle*>* c = particles.start; c != NULL; c = c->next)
	{
		if (bodyA == c->data->body && bodyB->colType == CollisionType::PLAYER)
		{
			if (c->data->type == KEY)
			{
				app->player->keyFound = true;
			}
			if (c->data->type == COIN)
			{
				app->player->points += 5;
			}
			if (c->data->type == HEART && app->player->lives == 2)
			{
				app->player->lives = 3;

			}
			if (c->data->type == HEART && app->player->lives == 1)
			{
				app->player->lives = 2;
				c->data->pendingToDelete = true;
			}

			c->data->pendingToDelete = true;
		}


	}
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
	particles.clear();
	return ret;
}