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

//#include "FadeToBlack.h"

#include <stdio.h>

Player::Player(bool startEnabled) : Module(startEnabled)
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

}

Player::~Player()
{

}

// Awake player
bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	// Load character's sprites
	// Easy for changing sprites of characters from xml and folders
	playerSprites.Create(config.child("spritesFolder").child_value());

	// Controls sprites for drawing in screen
	controls.Create(config.child("controls").child_value());
	tutorials.Create(config.child("tutorials").child_value());

	// Player's initial position saved in xml
	startPos.x = config.child("startPosition").attribute("x").as_int();
	startPos.y = config.child("startPosition").attribute("y").as_int();

	// Number of the player's jump
	jump = config.child("jumps").attribute("value").as_int();

	// Player's speed
	speed = config.child("speed").attribute("value").as_int();

	return ret;
}

bool Player::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	// Textures
	texture = app->tex->Load(playerSprites.GetString());
	controlsTex = app->tex->Load(controls.GetString());
	tutorialsTex = app->tex->Load(tutorials.GetString());

	// stating animation
	currentAnimation = &idleAnimR;

	// L10: DONE 4: Retrieve the player when playing a second time
	controlsVisible = false;
	tutorialVisible = false;
	chestFound = false;
	chestOpen = false;
	GodMode = false;
	checkPoint = false;
	checkPointReached = false;
	checkPointCollision = false;
	win = false;
	lives = 3;

	position.x = startPos.x;
	position.y = startPos.y;

	// Create Player
	playerBody = app->physics->CreateChain(position.x, position.y, playerHitbox, 16, 0);
	playerBody->listener = this;
	playerBody->colType = CollisionType::PLAYER;

	// Create Sensor for player
	b2FixtureDef playerHitbox;
	b2PolygonShape playerSensor;
	b2Vec2 vec(PIXEL_TO_METERS(12), PIXEL_TO_METERS(32));
	playerSensor.SetAsBox(PIXEL_TO_METERS(12), PIXEL_TO_METERS(8),vec,0);
	playerHitbox.shape = &playerSensor;
	playerHitbox.isSensor = true;
	playerBody->body->CreateFixture(&playerHitbox);

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	onGround = false;
	if (playerBody->body->GetLinearVelocity().y == 0) onGround = true;

	// Position of player is restarted if game is restarted
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		playerBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
	}
	
	// L10: DONE: Implement gamepad support
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		b2Vec2 vel = playerBody->body->GetLinearVelocity();
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
		playerBody->body->SetLinearVelocity(vel);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = playerBody->body->GetLinearVelocity();
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
		playerBody->body->SetLinearVelocity(vel);
	}

	if (playerBody->body->GetLinearVelocity().y == 0) jump = 2;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && playerBody->body->GetLinearVelocity().y >= 0 && jump != 0)
	{
		jump--;
		playerBody->body->SetLinearVelocity({ playerBody->body->GetLinearVelocity().x, -5.0f });
	}

	if (onGround == false)
	{
		if (playerBody->body->GetLinearVelocity().y < -1.0f && playerBody->body->GetLinearVelocity().x < -0.1f ||
			playerBody->body->GetLinearVelocity().y > 0.1f && playerBody->body->GetLinearVelocity().x < 0.1f)
		{
			if (currentAnimation != &jumpL)
			{
				jumpL.Reset();
				currentAnimation = &jumpL;
			}
		}
		if (playerBody->body->GetLinearVelocity().y < -1.0f && playerBody->body->GetLinearVelocity().x > 0.1f ||
			playerBody->body->GetLinearVelocity().y > 0.1f && playerBody->body->GetLinearVelocity().x > -0.1f)
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

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) { controlsVisible = !controlsVisible; tutorialVisible = !tutorialVisible; }

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
		if (position.y > 1550)
		{
			lives-=1;
			if (lives > 0)
			{
				if (checkPointReached == false)
				{
					playerBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
					LOG("lives count: %i", lives);
				}
				else
				{
					playerBody->body->SetTransform({ PIXEL_TO_METERS(checkPointPos.x), PIXEL_TO_METERS(checkPointPos.y) }, 0.0f);
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

	if (chestFound == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			chestOpen = true;
			LOG("chest opened yaaaay!");
		}
	}
	currentAnimation->Update();

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	tutRect = JumpTut.GetCurrentFrame();
	chestRect = openTut.GetCurrentFrame();
	ladderRect = ladderTut.GetCurrentFrame();
	playerBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x-2, position.y-4, &rect);
	return ret;
}

void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Ladders doesn't work for now
	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::LADDER)
	{
		LOG("it works fst part");
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT  && playerBody->body->GetLinearVelocity().y < -0.25f)
		{
			playerBody->body->ApplyLinearImpulse({ 0,-0.16f }, { 0,0 }, true);
			LOG("it works scnd part");
		}
		
	}

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::STONEWIN)
	{
		if (chestOpen == true) 
		{
			LOG("you won!");
			win = true;
		}
	}

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CONTROLS)
	{
		LOG("these are the controls");
		controlsVisible = true;
	}
	else controlsVisible = false;

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::TUORIALS)
	{
		LOG("tutorial!");
		tutorialVisible = true;
	}
	else tutorialVisible = false;

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CHEST)
	{
		LOG("open chest!");
		chestFound = true;
	}
	else chestFound = false;

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::CHECKPOINT)
	{
		LOG("Checkpoint reached!");
		checkPoint = true;
	}

	//ENEMY LOGIC

	if (bodyA->colType == CollisionType::PLAYER && bodyB->colType == CollisionType::DOG)
	{
		if (onGround == true)
		{
			LOG("THE DOG BIT YOU!");
		}
		else
		{
			LOG("YOU KILLED THE DOG!");
			app->enemy->dogDead = true;
			playerBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
		}
	}
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	playerBody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
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
	app->physics->world->DestroyBody(playerBody->body);
	return ret;
}