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

Player::Player() : Module()
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
	b->SetUserData(playerBody);
	playerBody->width = playerBody->height = playerCircle.m_radius;
	playerBody->listener = this;
	//---------------------------------------------------------------------------//


	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	// Position of player is restarted if game is restarted
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		playerBody->body->SetTransform({ PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
	}
	
	// L10: DONE: Implement gamepad support
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (playerBody->body->GetLinearVelocity().x >= -2) playerBody->body->ApplyLinearImpulse({ -1.0f,0 }, { 0,0 }, true);
		if (currentAnimation != &leftAnim)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (playerBody->body->GetLinearVelocity().x <= +2) playerBody->body->ApplyLinearImpulse({ 1.0f,0 }, { 0,0 }, true);
		if (currentAnimation != &rightAnim)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
	}

	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//{
	//	position.y += speed;
	//	if (currentAnimation != &leftAnim)
	//	{
	//		leftAnim.Reset();
	//		currentAnimation = &leftAnim;
	//	}
	//}

	//if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	position.y -= speed;
	//	if (currentAnimation != &leftAnim)
	//	{
	//		leftAnim.Reset();
	//		currentAnimation = &leftAnim;
	//	}
	//}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		playerBody->body->ApplyLinearImpulse({ 0,-2 }, { 0,0 }, true);
	}

	//if (app->input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN && jump == false && inTheAir == false)
	//{
	//	Player->body->ApplyLinearImpulse({ 0,-160 }, { 0,0 }, true);
	//	app->audio->PlayFx(jumpSound);
	//	//jump = true;
	//}

	// Player bounds
	//if (position.x == -4476)
	//{
	//	position.x = -4476;
	//}
	//if (position.x == 0)
	//{
	//	position.x = 0;
	//}
	//if (position.y == -2319)
	//{
	//	position.y = -2319;
	//}
	//if (position.y == 0)
	//{
	//	position.y = 0;
	//}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (currentAnimation == &rightAnim)
		{
			currentAnimation = &idleAnimR;
		}
		if (currentAnimation == &leftAnim)
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

}

bool Player::LoadState(pugi::xml_node& data)
{
	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	return true;
}

bool Player::CleanUp()
{
	bool ret = true;
	app->tex->UnLoad(texture);
	app->physics->world->DestroyBody(b);
	return ret;
}