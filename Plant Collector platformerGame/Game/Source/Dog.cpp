#include "Dog.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "Module.h"
#include "Log.h"
#include "Physics.h"
#include "Enemy.h"
#include "Player.h"
#include "EntityManager.h"

//#include "FadeToBlack.h"

#include <stdio.h>

Dog::Dog(iPoint position_) : Entity(EntityType::DOG, position_)
{
	name.Create("dog");

	//Dog Animation
	dogsR.PushBack({ 0, 192, 64, 64 });
	dogsR.PushBack({ 64, 192, 64, 64 });
	dogsR.PushBack({ 128, 192, 64, 64 });
	dogsR.PushBack({ 192, 192, 64, 64 });
	dogsR.loop = true;
	dogsR.speed = 0.05f;

	dogsL.PushBack({ 0, 256, 64, 64 });
	dogsL.PushBack({ 64, 256, 64, 64 });
	dogsL.PushBack({ 128, 256, 64, 64 });
	dogsL.PushBack({ 192, 256, 64, 64 });
	dogsL.loop = true;
	dogsL.speed = 0.05f;

	dogsBarkR.PushBack({ 192, 320, 64, 64 });
	dogsBarkR.PushBack({ 128, 320, 64, 64 });
	dogsBarkR.PushBack({ 64, 320, 64, 64 });
	dogsBarkR.PushBack({ 0, 320, 64, 64 });
	dogsBarkR.loop = true;
	dogsBarkR.speed = 0.05f;

	dogsBarkL.PushBack({ 0, 0, 64, 64 });
	dogsBarkL.PushBack({ 64, 0, 64, 64 });
	dogsBarkL.PushBack({ 128, 0, 64, 64 });
	dogsBarkL.PushBack({ 192, 0, 64, 64 });
	dogsBarkL.loop = true;
	dogsBarkL.speed = 0.05f;

	dogsRunR.PushBack({ 0, 64, 64, 64 });
	dogsRunR.PushBack({ 64, 64, 64, 64 });
	dogsRunR.PushBack({ 128, 64, 64, 64 });
	dogsRunR.PushBack({ 192, 64, 64, 64 });
	dogsRunR.PushBack({ 256, 64, 64, 64 });
	dogsRunR.PushBack({ 320, 64, 64, 64 });
	dogsRunR.loop = true;
	dogsRunR.speed = 0.15f;

	dogsRunL.PushBack({ 320, 128, 64, 64 });
	dogsRunL.PushBack({ 256, 128, 64, 64 });
	dogsRunL.PushBack({ 192, 128, 64, 64 });
	dogsRunL.PushBack({ 128, 128, 64, 64 });
	dogsRunL.PushBack({ 64, 128, 64, 64 });
	dogsRunL.PushBack({ 0, 128, 64, 64 });
	dogsRunL.loop = true;
	dogsRunL.speed = 0.15f;

	dogsDieR.PushBack({ 0, 384, 64, 64 });
	dogsDieR.PushBack({ 64, 384, 64, 64 });
	dogsDieR.PushBack({ 128, 384, 64, 64 });
	dogsDieR.PushBack({ 192, 384, 64, 64 });
	dogsDieR.PushBack({ 256, 384, 64, 64 });
	dogsDieR.loop = false;
	dogsDieR.speed = 0.05f;

	dogsDieL.PushBack({ 192, 448, 64, 64 });
	dogsDieL.PushBack({ 128, 448, 64, 64 });
	dogsDieL.PushBack({ 64, 448, 64, 64 });
	dogsDieL.PushBack({ 0, 448, 64, 64 });
	dogsDieL.PushBack({ 256, 448, 64, 64 });
	dogsDieL.loop = false;
	dogsDieL.speed = 0.05f;

	dogsHurtR.PushBack({ 0, 512, 64, 64 });
	dogsHurtR.PushBack({ 64, 512, 64, 64 });
	dogsHurtR.loop = true;
	dogsHurtR.speed = 0.05f;

	dogsHurtL.PushBack({ 64, 576, 64, 64 });
	dogsHurtL.PushBack({ 0, 576, 64, 64 });
	dogsHurtL.loop = true;
	dogsHurtL.speed = 0.05f;

	currentAnimation = &dogsR;

	//body
	pBody = app->physics->CreateChain(position_.x, position_.y, Hitbox, 16, 0);
	pBody->entityListener = this;
	pBody->colType = CollisionType::DOGCOL;
	dogDead = false;

	startPos.x = position_.x;
	startPos.y = position_.y;

}

Dog::~Dog()
{

}

bool Dog::Update(float dt)
{
	bool ret = true;

	position.x = METERS_TO_PIXELS(pBody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pBody->body->GetPosition().y);

	if (dogDead == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			pBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
		}

		velocity = pBody->body->GetLinearVelocity();

		if (position.DistanceTo(app->entityManager->player->position) < 300)
		{
			if (position.x < app->entityManager->player->position.x && (position.y > app->entityManager->player->position.y ||position.y == app->entityManager->player->position.y))
			{
				currentAnimation = &dogsRunR;
				pBody->body->SetLinearVelocity({ 2.5f,0.0f });
			}
			if (position.x > app->entityManager->player->position.x && (position.y > app->entityManager->player->position.y || position.y == app->entityManager->player->position.y))
			{
				currentAnimation = &dogsRunL;
				pBody->body->SetLinearVelocity({ -2.5f,0.0f });

			}
		}
		else
		{
			b2Vec2 Vel0;
			Vel0 = { 0.0f,0.0f };

			if (position.x == startPos.x)
			{
				if (currentAnimation == &dogsRunR)
				{
					velocity = Vel0;
					currentAnimation = &dogsR;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
					if (velocity == Vel0) currentAnimation = &dogsR;
				}
				if (currentAnimation == &dogsRunL)
				{
					velocity = Vel0;
					currentAnimation = &dogsL;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
					if (velocity == Vel0) currentAnimation = &dogsR;
				}
			}
			if (position.x < startPos.x)
			{
				currentAnimation = &dogsRunR;
				pBody->body->SetLinearVelocity({ 1.5f,0.0f });
			}
			if (position.x > startPos.x)
			{
				currentAnimation = &dogsRunL;
				pBody->body->SetLinearVelocity({ -1.5f,0.0f });

			}
		}
	}
	else
	{
		if (currentAnimation != &dogsRunR)
		{
			dogsRunR.Reset();
			currentAnimation = &dogsDieR;
		}
		if (currentAnimation != &dogsRunL)
		{
			dogsRunL.Reset();
			currentAnimation = &dogsDieL;
		}
	}

	currentAnimation->Update();
	return ret;
}

bool Dog::LoadState(pugi::xml_node& data)
{
	position.x = data.child("dogPosition").attribute("x").as_int();
	position.y = data.child("dogPosition").attribute("y").as_int();
	pBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	return true;
}

bool Dog::SaveState(pugi::xml_node& data) const
{
	data.child("dogPosition").attribute("x").set_value(position.x);
	data.child("dogPosition").attribute("y").set_value(position.y);
	return true;
}

bool Dog::CleanUp()
{
	LOG("Destroying Dog");
	bool ret = true;
	app->physics->world->DestroyBody(pBody->body);
	return ret;
}
