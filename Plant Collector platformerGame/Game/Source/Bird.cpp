#include "Bird.h"
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

Bird::Bird(iPoint position_) : Entity(EntityType::BIRD, position_)
{
	name.Create("bird");

	//Bird Animation
	birdR.PushBack({ 0, 128, 64, 64 });
	birdR.PushBack({ 64, 128, 64, 64 });
	birdR.PushBack({ 128, 128, 64, 64 });
	birdR.PushBack({ 192, 128, 64, 64 });
	birdR.loop = true;
	birdR.speed = 0.05f;

	birdL.PushBack({ 192, 192, 64, 64 });
	birdL.PushBack({ 128, 192, 64, 64 });
	birdL.PushBack({ 64, 192, 64, 64 });
	birdL.PushBack({ 0, 192, 64, 64 });
	birdL.loop = true;
	birdL.speed = 0.05f;

	birdHurtR.PushBack({ 0, 256, 64, 64 });
	birdHurtR.PushBack({ 64, 256, 64, 64 });
	birdHurtR.loop = true;
	birdHurtR.speed = 0.05f;

	birdHurtL.PushBack({ 64, 320, 64, 64 });
	birdHurtL.PushBack({ 0, 320, 64, 64 });
	birdHurtL.loop = true;
	birdHurtL.speed = 0.05f;

	birdDieR.PushBack({ 0, 384, 64, 64 });
	birdDieR.PushBack({ 64, 384, 64, 64 });
	birdDieR.PushBack({ 128, 384, 64, 64 });
	birdDieR.PushBack({ 192, 384, 64, 64 });
	birdDieR.PushBack({ 256, 384, 64, 64 });
	birdDieR.PushBack({ 400, 500, 64, 64 });
	birdDieR.loop = false;
	birdDieR.speed = 0.05f;

	birdDieL.PushBack({ 192, 448, 64, 64 });
	birdDieL.PushBack({ 128, 448, 64, 64 });
	birdDieL.PushBack({ 64, 448, 64, 64 });
	birdDieL.PushBack({ 0, 448, 64, 64 });
	birdDieL.PushBack({ 256, 448, 64, 64 });
	birdDieL.PushBack({ 400, 500, 64, 64 });
	birdDieL.loop = false;
	birdDieL.speed = 0.05f;

	birdFlyL.PushBack({ 320, 448, 64, 64 });
	birdFlyL.PushBack({ 256, 448, 64, 64 });
	birdFlyL.PushBack({ 192, 448, 64, 64 });
	birdFlyL.PushBack({ 128, 448, 64, 64 });
	birdFlyL.PushBack({ 64, 448, 64, 64 });
	birdFlyL.PushBack({ 0, 448, 64, 64 });
	birdFlyL.loop = true;
	birdFlyL.speed = 0.15f;

	birdFlyR.PushBack({ 0, 384, 64, 64 });
	birdFlyR.PushBack({ 64, 384, 64, 64 });
	birdFlyR.PushBack({ 128, 384, 64, 64 });
	birdFlyR.PushBack({ 192, 384, 64, 64 });
	birdFlyR.PushBack({ 256, 384, 64, 64 });
	birdFlyR.PushBack({ 320, 384, 64, 64 });
	birdFlyR.loop = true;
	birdFlyR.speed = 0.15f;

	currentAnimation = &birdL;

	//body
	pBody = app->physics->CreateBirdChain(position_.x, position_.y, Hitbox, 16, 0);
	pBody->entityListener = this;
	pBody->colType = CollisionType::BIRDCOL;
	active = true;

	startPos.x = position_.x;
	startPos.y = position_.y;

}

Bird::~Bird()
{

}

bool Bird::Update(float dt)
{
	bool ret = true;

	position.x = METERS_TO_PIXELS(pBody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pBody->body->GetPosition().y);

	// Position of cat is restarted if game is restarted
	if (active == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			pBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
		}

		velocity = pBody->body->GetLinearVelocity();

		pBody->body->SetLinearVelocity(velocity);

		if (position.DistanceTo(app->entityManager->player->position) < 300)
		{
			if (position.x < app->entityManager->player->position.x)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,0.0f });

			}
			if (position.x > app->entityManager->player->position.x)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,0.0f });

			}
			if (position.y > app->entityManager->player->position.y)
			{
				pBody->body->SetLinearVelocity({ 0.0f,-0.5f });
			}
			if (position.y < app->entityManager->player->position.y)
			{
				pBody->body->SetLinearVelocity({ 0.0f,0.5f });
			}
			if (position.x < app->entityManager->player->position.x && position.y > app->entityManager->player->position.y)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,-0.5f });

			}
			if (position.x > app->entityManager->player->position.x && position.y > app->entityManager->player->position.y)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,-0.5f });

			}
			if (position.x < app->entityManager->player->position.x && position.y < app->entityManager->player->position.y)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,0.5f });

			}
			if (position.x > app->entityManager->player->position.x && position.y < app->entityManager->player->position.y)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,0.5f });

			}
		}
		else
		{
			if (position.x == position.x && position.y == startPos.y)
			{
				if (currentAnimation == &birdFlyR)
				{
					currentAnimation = &birdR;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
				}
				if (currentAnimation == &birdFlyL)
				{
					currentAnimation = &birdL;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
				}
			}
			if (position.x < startPos.x)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,0.0f });
			}
			if (position.x > startPos.x)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,0.0f });

			}
			if (position.y > startPos.y)
			{
				pBody->body->SetLinearVelocity({ 0.0f,-0.5f });
			}
			if (position.y < startPos.y)
			{
				pBody->body->SetLinearVelocity({ 0.0f,0.5f });
			}
			if (position.x < startPos.x && position.y >  startPos.y)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,-0.5f });

			}
			if (position.x > startPos.x && position.y > startPos.y)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,-0.5f });

			}
			if (position.x < startPos.x && position.y < startPos.y)
			{
				currentAnimation = &birdFlyR;
				pBody->body->SetLinearVelocity({ 0.7f,0.5f });

			}
			if (position.x > startPos.x && position.y < startPos.y)
			{
				currentAnimation = &birdFlyL;
				pBody->body->SetLinearVelocity({ -0.7f,0.5f });

			}
		}
	}
	else
	{
		if (currentAnimation != &birdFlyR)
		{
			birdFlyR.Reset();
			currentAnimation = &birdDieR;
			pBody->body->SetLinearVelocity({ 0.0f,0.0f });
		}
		if (currentAnimation != &birdFlyL)
		{
			birdFlyL.Reset();
			currentAnimation = &birdDieL;
			pBody->body->SetLinearVelocity({ 0.0f,0.0f });
		}
	}

	currentAnimation->Update();
	return ret;
}

bool Bird::LoadState(pugi::xml_node& data)
{
	position.x = data.child("birdPosition").attribute("x").as_int();
	position.y = data.child("birdPosition").attribute("y").as_int();
	pBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	return true;
}

bool Bird::SaveState(pugi::xml_node& data) const
{
	data.child("birdPosition").attribute("x").set_value(position.x);
	data.child("birdPosition").attribute("y").set_value(position.y);
	return true;
}

bool Bird::CleanUp()
{
	LOG("Destroying Bird");
	bool ret = true;
	app->physics->world->DestroyBody(pBody->body);
	return ret;
}