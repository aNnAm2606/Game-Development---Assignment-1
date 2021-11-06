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
#include "Scene.h"

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

	texture = app->tex->Load(playerSprites.GetString());
	
	// stating animation
	currentAnimation = &idleAnimR;

	// L10: DONE 4: Retrieve the player when playing a second time
	dead = false;
	GodMode = false;
	win = false;

	position.x = startPos.x;
	position.y = startPos.y;

	/*                                    BOX2D                                */
	//-------------------------------------------------------------------------//
	// Player body, shape and fixture with Box2D
	b2BodyDef pbody;
	pbody.type = b2_dynamicBody;
	pbody.position.Set(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
	pbody.fixedRotation = true;
	//create the body in  the world
	b = app->physics->world->CreateBody(&pbody);
	////add a shape
	//b2PolygonShape playerSquare;
	// 32x32 is the character's dimension in px
	playerCircle.m_radius = PIXEL_TO_METERS(14);
	//add fixture
	b2FixtureDef playerfixture;
	playerfixture.shape = &playerCircle;
	playerfixture.density = 1.5f;
	playerfixture.friction = 100.0f;
	//add fixture to body
	b->CreateFixture(&playerfixture);
	// Create our custom PhysBody class
	playerBody = new PhysBody();
	playerBody->body =b;
	playerBody->width = playerBody->height = playerCircle.m_radius;
	playerBody->listener = this;
	playerBody->colType = PLAYER;
	b->SetUserData(playerBody);
	//---------------------------------------------------------------------------//


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
		playerBody->body->SetLinearVelocity(vel);
		if (onGround == true)
		{
			if (currentAnimation != &leftAnim)
			{
				leftAnim.Reset();
				currentAnimation = &leftAnim;
			}
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = playerBody->body->GetLinearVelocity();
		vel.x = 3.0f;
		playerBody->body->SetLinearVelocity(vel);
		if (onGround == true)
		{
			if (currentAnimation != &rightAnim)
			{
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}
		}
	}

	if (playerBody->body->GetLinearVelocity().y == 0) jump = 2;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && playerBody->body->GetLinearVelocity().y >= 0 && jump != 0)
	{
		jump--;
		playerBody->body->SetLinearVelocity({ playerBody->body->GetLinearVelocity().x, -5.0f });
	}

	if (onGround == false)
	{
		if (playerBody->body->GetLinearVelocity().y < -1.0f && playerBody->body->GetLinearVelocity().x > 0.1f ||
			playerBody->body->GetLinearVelocity().y > 0.1f && playerBody->body->GetLinearVelocity().x > -0.1f)
		{
			if (currentAnimation != &jumpR)
			{
				jumpR.Reset();
				currentAnimation = &jumpR;
			}
		}
		if (playerBody->body->GetLinearVelocity().y < -1.0f && playerBody->body->GetLinearVelocity().x < -0.1f ||
			playerBody->body->GetLinearVelocity().y > 0.1f && playerBody->body->GetLinearVelocity().x < 0.1f)
		{
			if (currentAnimation != &jumpL)
			{
				jumpL.Reset();
				currentAnimation = &jumpL;
			}
		}
	}

	if (onGround == true && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (currentAnimation == &rightAnim)
		{
			currentAnimation = &idleAnimR;
		}
		if (currentAnimation == &leftAnim)
		{
			currentAnimation = &idleAnimL;
		}
		if (currentAnimation == &jumpR)
		{
			currentAnimation = &idleAnimR;
		}
		if (currentAnimation == &jumpL)
		{
			currentAnimation = &idleAnimL;
		}
	}

	currentAnimation->Update();

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	playerBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x-16, position.y-16, &rect);
	return ret;
}

void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->colType == collisionType::PLAYER && bodyB->colType == collisionType::WALL)
	{
		LOG("it works fst part");
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			playerBody->body->ApplyLinearImpulse({ 0,-5 }, { 0,0 }, true);
			LOG("it works scnd part");
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
	bool ret = true;
	app->tex->UnLoad(texture);
	app->physics->world->DestroyBody(b);
	return ret;
}