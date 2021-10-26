#include "Player.h"
#include "Physics.h"
#include "Input.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"
#include "Log.h"
#include "Window.h"
#include "Audio.h"
#include "Map.h"
#include"Animation.h"



Player::Player(bool startenabled) 
{
	name.Create("player");
}

Player::~Player()
{
	// Player's animation and sprites
}

bool Player::Start()
{
	LOG("loading player textures");

	bool ret = true;

	// Player's texture loaders and audio/fx
	// NOTE: get the texture path and load it from the xml config child value as string
	// This helps us change the sprites directly from the files in the xml file
	// NOTE: the same for audio

	/* texture = app->tex->Load( path of the folder)*/

	// Player's body, shape and fixture with box2D
	// create a body
	b2BodyDef body;
	// set body type

	body.type = b2_dynamicBody;

	// set position in pixel to meters(x,y)
	body.position.Set(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));

	//fixed rotation
	body.fixedRotation = true;

	// create body in the world with b2Body* b = app->physics->world->CreateBody(&nameofbody)
	b2Body* b = app->physics->world->CreateBody(&body);

	// create shape in pixels to meters
	b2PolygonShape rectangle;
	rectangle.SetAsBox(25,25);

	// create a fixture
	b2FixtureDef fixture;

	// link the fixture to the shape
	fixture.shape = &rectangle;

	// set density and friction
	fixture.density = 20.0f;
	fixture.friction = 100.0f;

	// link the fixture to the body
	b->ResetMassData();
	b->CreateFixture(&fixture);

	return ret;
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	// Player's position
	//pos.x = config.child("pos").attribute("x").as_int();
	//pos.y = config.child("pos").attribute("y").as_int();

	// Player's important info and path to file of sprites
	/*numJumps = config.child("num_jumps").attribute("value").as_int();
	minVel = config.child("min_vel").attribute("value").as_float();
	maxVel = config.child("max_vel").attribute("value").as_float();
	jumpVel = config.child("jump_vel").attribute("value").as_float();
	folder.Create(config.child("folder").child_value());
	jumpSFXFile.Create(config.child("jump_SFX").child_value());*/

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;
	
	// debug key for gamepad rumble testing purposes
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		//debug mode on/off
	}

	// player controls with SDL_SCANCODE_:::
	//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;

	//get the current animation
	SDL_Rect rect = currentanimation->GetCurrentFrame();
	pbody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return ret;
}

//void Player::oncollision(collider* c1, collider* c2)
//{
//}

bool Player::LoadState(pugi::xml_node& data)
{
	bool ret = true;
	position.x = data.child("pos").attribute("x").as_int();
	position.y = data.child("pos").attribute("y").as_int();
	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);

	return ret;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	bool ret = true;
	pugi::xml_node posN = data.append_child("pos");
	posN.append_attribute("x").set_value(position.x);
	posN.append_attribute("y").set_value(position.y);

	return true;
}
