#include "Player.h"

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
#include "EntityManager.h"

//#include "FadeToBlack.h"

#include <stdio.h>

Player::Player(iPoint position_) : Entity(EntityType::PLAYER, position_)
{
	name.Create("player");
	
	// Player's animation:
	// idle animation - just one sprite
	idleAnimR.PushBack({ 0, 64, 32, 32 });
	idleAnimR.PushBack({ 32, 64, 32, 32 });
	idleAnimR.PushBack({ 64, 64, 32, 32 });
	idleAnimR.PushBack({ 96, 64, 32, 32 });
	idleAnimR.loop = true;
	idleAnimR.speed = 0.05f;

	idleAnimL.PushBack({ 0, 224, 32, 32 });
	idleAnimL.PushBack({ 32, 224, 32, 32 });
	idleAnimL.PushBack({ 64, 224, 32, 32 });
	idleAnimL.PushBack({ 96, 224, 32, 32 });
	idleAnimL.loop = true;
	idleAnimL.speed = 0.05f;

	rightAnim.PushBack({  0, 0, 32, 32 });
	rightAnim.PushBack({ 32, 0, 32, 32 });
	rightAnim.PushBack({ 64, 0, 32, 32 });
	rightAnim.PushBack({ 96, 0, 32, 32 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	leftAnim.PushBack({  0, 160, 32, 32 });
	leftAnim.PushBack({ 32, 160, 32, 32 });
	leftAnim.PushBack({ 64, 160, 32, 32 });
	leftAnim.PushBack({ 96, 160, 32, 32 });
	leftAnim.loop = true;
	leftAnim.speed = 0.1f;

	rightAnimShift.PushBack({ 0, 0, 32, 32 });
	rightAnimShift.PushBack({ 32, 0, 32, 32 });
	rightAnimShift.PushBack({ 64, 0, 32, 32 });
	rightAnimShift.PushBack({ 96, 0, 32, 32 });
	rightAnimShift.loop = true;
	rightAnimShift.speed = 0.2f;

	leftAnimShift.PushBack({ 0, 160, 32, 32 });
	leftAnimShift.PushBack({ 32, 160, 32, 32 });
	leftAnimShift.PushBack({ 64, 160, 32, 32 });
	leftAnimShift.PushBack({ 96, 160, 32, 32 });
	leftAnimShift.loop = true;
	leftAnimShift.speed = 0.2f;

	jumpR.PushBack({ 0, 32, 32, 32 });
	jumpR.PushBack({ 32, 32, 32, 32 });
	jumpR.PushBack({ 64, 32, 32, 32 });
	jumpR.PushBack({ 96, 32, 32, 32 });
	jumpR.loop = false;
	jumpR.speed = 0.05f;

	jumpL.PushBack({ 0, 192, 32, 32 });
	jumpL.PushBack({ 32, 192, 32, 32 });
	jumpL.PushBack({ 64, 192, 32, 32 });
	jumpL.PushBack({ 96, 192, 32, 32 });
	jumpL.loop = false;
	jumpL.speed = 0.05f;

	JumpTut.PushBack({ 0, 32, 96, 32 });

	ladderTut.PushBack({0, 0, 160,32 });

	openTut.PushBack({ 0, 64, 96, 32 });

	currentAnimation = &idleAnimR;

	// Create Player
	pBody = app->physics->CreateChain(position_.x, position_.y, playerHitbox, 16, 0);
	this->pBody->entityListener = this;
	pBody->colType = CollisionType::PLAYERCOL;

	startPos.x = position.x;
	startPos.y = position.y;

	chestOpen = false;
}

Player::~Player()
{

}

bool Player::Update(float dt)
{
	bool ret = true;

	position.x = METERS_TO_PIXELS(pBody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pBody->body->GetPosition().y);

	onGround = false;
	if (pBody->body->GetLinearVelocity().y == 0) onGround = true;

	// Position of player is restarted if game is restarted
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		pBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
	}
	
	// L10: DONE: Implement gamepad support
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		b2Vec2 vel = pBody->body->GetLinearVelocity();
		vel.x = -3.0f;
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			if (onGround == true)
			{
				vel.x = -5.0f;
				if (currentAnimation != &leftAnimShift)
				{
					leftAnim.Reset();
					currentAnimation = &leftAnimShift;
				}
			}
		}
		else
		{
			if (onGround == true)
			{
				vel.x = -3.0f;
				if (currentAnimation != &leftAnim)
				{
					leftAnim.Reset();
					currentAnimation = &leftAnim;
				}
			}
		}
		pBody->body->SetLinearVelocity(vel);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = pBody->body->GetLinearVelocity();
		vel.x = 3.0f;
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			if (onGround == true)
			{
				vel.x = 5.0f;
				if (currentAnimation != &rightAnimShift)
				{
					leftAnim.Reset();
					currentAnimation = &rightAnimShift;
				}
			}
		}
		else
		{
			if (onGround == true)
			{
				vel.x = 3.0f;
				if (currentAnimation != &rightAnim)
				{
					rightAnim.Reset();
					currentAnimation = &rightAnim;
				}
			}
		}
		pBody->body->SetLinearVelocity(vel);
	}

	if (pBody->body->GetLinearVelocity().y == 0) jump = 2;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && pBody->body->GetLinearVelocity().y >= 0 && jump != 0)
	{
		jump--;
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -5.0f });
	}

	if (onGround == false)
	{
		if (pBody->body->GetLinearVelocity().y < -1.0f && pBody->body->GetLinearVelocity().x < -0.1f ||
			pBody->body->GetLinearVelocity().y > 0.1f && pBody->body->GetLinearVelocity().x < 0.1f)
		{
			if (currentAnimation != &jumpL)
			{
				jumpL.Reset();
				currentAnimation = &jumpL;
			}
		}
		if (pBody->body->GetLinearVelocity().y < -1.0f && pBody->body->GetLinearVelocity().x > 0.1f ||
			pBody->body->GetLinearVelocity().y > 0.1f && pBody->body->GetLinearVelocity().x > -0.1f)
		{
			if (currentAnimation != &jumpR)
			{
				jumpR.Reset();
				currentAnimation = &jumpR;
			}
		}
	}

	if (onGround == true && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && 
		app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_IDLE)
	{
		if (currentAnimation == &rightAnim || currentAnimation == &jumpR)
		{
			currentAnimation = &idleAnimR;
		}
		if (currentAnimation == &leftAnim || currentAnimation == &jumpL)
		{
			currentAnimation = &idleAnimL;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		GodMode =!GodMode;
		if (GodMode == true)
		{
			LOG("God mode ON!");
			lives = 3;
		}
		if (GodMode == false) LOG("Gode mode OFF!");
	}

	if (GodMode == false)
	{
		if (playerHit == true)
		{
			pBody->body->SetTransform({ PIXEL_TO_METERS(checkPointPos.x), PIXEL_TO_METERS(checkPointPos.y) }, 0.0f);
			lives--;
			playerHit = false;
		}
		if (position.y > 1550)
		{
			lives-=1;
			if (lives > 0)
			{
				if (checkPointReached == false)
				{
					pBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
					LOG("lives count: %i", lives);
				}
				else
				{
					pBody->body->SetTransform({ PIXEL_TO_METERS(checkPointPos.x), PIXEL_TO_METERS(checkPointPos.y) }, 0.0f);
					LOG("lives count: %i", lives);
				}
			}
		}
	}

	if (checkPoint == true)
	{
		checkPointPos.x = position.x;
		checkPointPos.y = position.y;
		checkPoint = false;
		checkPointReached = true;
	}
	currentAnimation->Update();

	return ret;
}


//void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
//{
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::LADDER)
//	{
//		LOG("it works fst part");
//		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT  && playerBody->body->GetLinearVelocity().y < -0.25f)
//		{
//			playerBody->body->SetLinearVelocity({ 0,-5.0f });
//			LOG("it works scnd part");
//		}
//		
//	}
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::STONEWIN)
//	{
//		if (chestOpen == true) 
//		{
//			LOG("you won!");
//			win = true;
//		}
//	}
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CONTROLS)
//	{
//		LOG("these are the controls");
//		controlsVisible = true;
//	}
//	else controlsVisible = false;
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::TUORIALS)
//	{
//		LOG("tutorial!");
//		tutorialVisible = true;
//	}
//	else tutorialVisible = false;
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CHEST)
//	{
//		LOG("open chest!");
//		if(keyFound == true) chestFound = true;
//		else chestFound = false;
//	}
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CHECKPOINT)
//	{
//		LOG("Checkpoint reached!");
//		checkPoint = true;
//	}
//
//	//ENEMY LOGIC
//
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::DOG)
//	{
//		if (onGround == true)
//		{
//			LOG("THE DOG BIT YOU!");
//			if(GodMode == false && app->enemy->dogDead == false) playerHit = true;
//		}
//		else
//		{
//			LOG("YOU KILLED THE DOG!");
//			if(app->enemy->dogDead == false) 
//			{
//				app->audio->PlayFx(app->enemy->dogSound);
//				playerBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
//			}
//			app->enemy->dogDead = true;
//		}
//	}
//
//	// CAT LOGIC 
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CAT)
//	{
//		if (onGround == true)
//		{
//			LOG("THE CAT SCRATCHED YOU!");
//			if (GodMode == false && app->enemy->catDead == false) playerHit = true;
//		}
//		else
//		{
//			LOG("YOU KILLED THE CAT!");
//			if(app->enemy->catDead == false) 
//			{
//				app->audio->PlayFx(app->enemy->catSound);
//				playerBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
//			}
//			app->enemy->catDead = true;
//		}
//	}
//
//	// CAT LOGIC 
//	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::BIRD)
//	{
//		if (onGround == true)
//		{
//			LOG("BIRD KILLED YOU!");
//			if (GodMode == false && app->enemy->birdDead == false) playerHit = true;
//		}
//		else
//		{
//			LOG("YOU KILLED THE BIRD!");
//			if (app->enemy->birdDead == false)
//			{ 
//				app->audio->PlayFx(app->enemy->birdSound);
//				playerBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true); 
//			}
//			app->enemy->birdDead = true;
//		}
//	}
//
//}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	pBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	data.child("position").attribute("x").set_value(position.x);
	data.child("position").attribute("y").set_value(position.y);
	return true;
}

bool Player::CleanUp()
{
	LOG("Destroying Player");
	bool ret = true;
	app->tex->UnLoad(texture);
	app->physics->world->DestroyBody(pBody->body);
	return ret;
}