#include "Cat.h"
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

Cat::Cat(iPoint position_) : Entity(EntityType::CAT, position_)
{
	name.Create("cat");

	//Cat Animation
	catsR.PushBack({ 0, 128, 64, 64 });
	catsR.PushBack({ 64, 128, 64, 64 });
	catsR.PushBack({ 128, 128, 64, 64 });
	catsR.PushBack({ 192, 128, 64, 64 });
	catsR.loop = true;
	catsR.speed = 0.05f;

	catsL.PushBack({ 192, 192, 64, 64 });
	catsL.PushBack({ 128, 192, 64, 64 });
	catsL.PushBack({ 64, 192, 64, 64 });
	catsL.PushBack({ 0, 192, 64, 64 });
	catsL.loop = true;
	catsL.speed = 0.05f;

	catsHurtR.PushBack({ 0, 384, 64, 64 });
	catsHurtR.PushBack({ 64, 384, 64, 64 });
	catsHurtR.loop = true;
	catsHurtR.speed = 0.05f;

	catsHurtL.PushBack({ 64, 448, 64, 64 });
	catsHurtL.PushBack({ 0, 448, 64, 64 });
	catsHurtL.loop = true;
	catsHurtL.speed = 0.05f;

	catsDieR.PushBack({ 0, 256, 64, 64 });
	catsDieR.PushBack({ 64, 256, 64, 64 });
	catsDieR.PushBack({ 128, 256, 64, 64 });
	catsDieR.PushBack({ 192, 256, 64, 64 });
	catsDieR.PushBack({ 256, 256, 64, 64 });
	catsDieR.loop = false;
	catsDieR.speed = 0.05f;

	catsDieL.PushBack({ 192, 320, 64, 64 });
	catsDieL.PushBack({ 128, 320, 64, 64 });
	catsDieL.PushBack({ 64, 320, 64, 64 });
	catsDieL.PushBack({ 0, 320, 64, 64 });
	catsDieL.PushBack({ 256, 320, 64, 64 });
	catsDieL.loop = false;
	catsDieL.speed = 0.05f;

	catsRunL.PushBack({ 320, 576, 64, 64 });
	catsRunL.PushBack({ 256, 576, 64, 64 });
	catsRunL.PushBack({ 192, 576, 64, 64 });
	catsRunL.PushBack({ 128, 576, 64, 64 });
	catsRunL.PushBack({ 64, 576, 64, 64 });
	catsRunL.PushBack({ 0, 576, 64, 64 });
	catsRunL.loop = true;
	catsRunL.speed = 0.15f;

	catsRunR.PushBack({ 0, 512, 64, 64 });
	catsRunR.PushBack({ 64, 512, 64, 64 });
	catsRunR.PushBack({ 128, 512, 64, 64 });
	catsRunR.PushBack({ 192, 512, 64, 64 });
	catsRunR.PushBack({ 256, 512, 64, 64 });
	catsRunR.PushBack({ 320, 512, 64, 64 });
	catsRunR.loop = true;
	catsRunR.speed = 0.15f;

	currentAnimation = &catsR;

	//body
	pBody = app->physics->CreateChain(position_.x, position_.y, Hitbox, 16, 0);
	pBody->entityListener = this;
	pBody->colType = CollisionType::CATCOL;
	catDead = false;

	startPos.x = position_.x;
	startPos.y = position_.y;

}

Cat::~Cat()
{

}

bool Cat::Update(float dt)
{
	bool ret = true;

	position.x = METERS_TO_PIXELS(pBody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pBody->body->GetPosition().y);

	// Position of cat is restarted if game is restarted
	if (catDead == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			pBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
		}

		velocity = pBody->body->GetLinearVelocity();

		if (position.DistanceTo(app->entityManager->player->position) < 300)
		{
			if (position.x < app->entityManager->player->position.x && (position.y > app->entityManager->player->position.y || position.y == app->entityManager->player->position.y))
			{
				currentAnimation = &catsRunR;
				pBody->body->SetLinearVelocity({ 2.5f,0.0f });
			}
			if (position.x > app->entityManager->player->position.x && (position.y > app->entityManager->player->position.y || position.y == app->entityManager->player->position.y))
			{
				currentAnimation = &catsRunL;
				pBody->body->SetLinearVelocity({ -2.5f,0.0f });

			}
		}
		else
		{
			b2Vec2 Vel0;
			Vel0 = { 0.0f,0.0f };
			if (position.x == startPos.x)
			{
				velocity = Vel0;
				if (currentAnimation == &catsRunR)
				{
					currentAnimation = &catsR;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
					if (velocity == Vel0) currentAnimation = &catsR;
				}
				if (currentAnimation == &catsRunL)
				{
					currentAnimation = &catsL;
					pBody->body->SetLinearVelocity({ 0.0f,0.0f });
					if (velocity == Vel0) currentAnimation = &catsL;
				}
			}
			if (position.x < startPos.x)
			{
				currentAnimation = &catsRunR;
				pBody->body->SetLinearVelocity({ 1.5f,0.0f });
			}
			if (position.x > startPos.x)
			{
				currentAnimation = &catsRunL;
				pBody->body->SetLinearVelocity({ -1.5f,0.0f });
			}
		}
	}
	else
	{
		if (currentAnimation != &catsRunR)
		{
			catsRunR.Reset();
			currentAnimation = &catsDieR;
		}
		if (currentAnimation != &catsRunL)
		{
			catsRunL.Reset();
			currentAnimation = &catsDieL;
		}
	}

	currentAnimation->Update();
	return ret;
}

bool Cat::LoadState(pugi::xml_node& data)
{
	position.x = data.child("catPosition").attribute("x").as_int();
	position.y = data.child("catPosition").attribute("y").as_int();
	pBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	return true;
}

bool Cat::SaveState(pugi::xml_node& data) const
{
	data.child("catPosition").attribute("x").set_value(position.x);
	data.child("catPosition").attribute("y").set_value(position.y);
	return true;
}

bool Cat::CleanUp()
{
	LOG("Destroying Cat");
	bool ret = true;
	app->physics->world->DestroyBody(pBody->body);
	return ret;
}