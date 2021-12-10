#include "Enemy.h"

#include "Animation.h"
#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Player.h"
#include "Input.h"
#include "Textures.h"
#include "Module.h"
#include "Physics.h"

#include <stdlib.h>
#include "Defs.h"
#include "Log.h"

#include "External/Optick/include/optick.h"

Enemy::Enemy(bool startEnabled) : Module(startEnabled)
{
	name.Create("Enemy");

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
	dogsR.loop = true;
	dogsR.speed = 0.05f;

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
	birdDieR.loop = false;
	birdDieR.speed = 0.05f;

	birdDieL.PushBack({ 192, 448, 64, 64 });
	birdDieL.PushBack({ 128, 448, 64, 64 });
	birdDieL.PushBack({ 64, 448, 64, 64 });
	birdDieL.PushBack({ 0, 448, 64, 64 });
	birdDieL.PushBack({ 256, 448, 64, 64 });
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


	//Rat Animation
	ratsR.PushBack({ 0, 256, 64, 64 });
	ratsR.PushBack({ 64, 256, 64, 64 });
	ratsR.PushBack({ 128, 256, 64, 64 });
	ratsR.PushBack({ 192, 256, 64, 64 });
	ratsR.loop = true;
	ratsR.speed = 0.05f;

	ratsL.PushBack({ 192, 320, 64, 64 });
	ratsL.PushBack({ 128, 320, 64, 64 });
	ratsL.PushBack({ 64, 320, 64, 64 });
	ratsL.PushBack({ 0, 320, 64, 64 });
	ratsL.loop = true;
	ratsL.speed = 0.05f;

	ratsHurtR.PushBack({ 0, 128, 64, 64 });
	ratsHurtR.PushBack({ 64, 128, 64, 64 });
	ratsHurtR.loop = true;
	ratsHurtR.speed = 0.05f;

	ratsHurtL.PushBack({ 64, 192, 64, 64 });
	ratsHurtL.PushBack({ 0, 192, 64, 64 });
	ratsHurtL.loop = true;
	ratsHurtL.speed = 0.05f;

	ratsDieR.PushBack({ 0, 0, 64, 64 });
	ratsDieR.PushBack({ 64, 0, 64, 64 });
	ratsDieR.PushBack({ 128, 0, 64, 64 });
	ratsDieR.loop = false;
	ratsDieR.speed = 0.05f;


	ratsDieL.PushBack({ 64, 64, 64, 64 });
	ratsDieL.PushBack({ 0, 64, 64, 64 });
	ratsDieL.PushBack({ 128, 64, 64, 64 });
	ratsDieL.loop = false;
	ratsDieL.speed = 0.05f;

	ratsRunL.PushBack({ 192, 448, 64, 64 });
	ratsRunL.PushBack({ 128, 448, 64, 64 });
	ratsRunL.PushBack({ 64, 448, 64, 64 });
	ratsRunL.PushBack({ 0, 448, 64, 64 });
	ratsRunL.loop = true;
	ratsRunL.speed = 0.15f;

	ratsRunR.PushBack({ 0, 384, 64, 64 });
	ratsRunR.PushBack({ 64, 384, 64, 64 });
	ratsRunR.PushBack({ 128, 384, 64, 64 });
	ratsRunR.PushBack({ 192, 384, 64, 64 });
	ratsRunR.loop = true;
	ratsRunR.speed = 0.15f;

}

//Enemy::Enemy(int x, int y) : position(x, y)
//{
//	spawnPos = position;
//}

Enemy::~Enemy()
{
	/*if (collider != nullptr)*/
		/*collider->pendingToDelete = true;*/
}

bool Enemy::Awake(pugi::xml_node& config)
{
	LOG("Loading Enemy");
	bool ret = true;
	textureDog.Create(config.child("textureDog").child_value());
	textureBird.Create(config.child("textureBird").child_value());
	textureCat.Create(config.child("textureCat").child_value());
	textureRat.Create(config.child("textureRat").child_value());

	// Dogs's initial position saved in xml
	startPosDog.x = config.child("startPositionDog").attribute("x").as_int();
	startPosDog.y = config.child("startPositionDog").attribute("y").as_int();

	// Enemy's speed
	speed = config.child("speed").attribute("value").as_int();
	return ret;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

// Called before the first frame
bool Enemy::Start()
{
	// Load enemies
	
	dog = app->tex->Load(textureDog.GetString());
	bird = app->tex->Load(textureBird.GetString());
	cat = app->tex->Load(textureCat.GetString());
	rat = app->tex->Load(textureRat.GetString());

	// stating animation
	currentDogAnim = &dogsR;
	currentBirdAnim = &birdR;
	currentCatAnim = &catsR;
	currentRatAnim = &ratsR;

	position.x = startPosDog.x;
	position.y = startPosDog.y;

	/*                                    BOX2D                                */
	//-------------------------------------------------------------------------//
	// Player body, shape and fixture with Box2D
	b2BodyDef dogbody;
	dogbody.type = b2_staticBody;
	dogbody.position.Set(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
	dogbody.fixedRotation = true;
	//create the body in  the world
	bdog = app->physics->world->CreateBody(&dogbody);
	////add a shape
	//b2PolygonShape playerSquare;
	// 32x32 is the character's dimension in px
	dogCircle.m_radius = PIXEL_TO_METERS(12);;
	//add fixture
	b2FixtureDef dogfixture;
	dogfixture.shape = &dogCircle;
	dogfixture.density = 1.5f;
	dogfixture.friction = 100.0f;
	//add fixture to body
	bdog->CreateFixture(&dogfixture);
	// Create our custom PhysBody class
	dogBody = new PhysBody();
	dogBody->body = bdog;
	dogBody->width = dogBody->height = dogCircle.m_radius;
	dogBody->listener = this;
	dogBody->colType = collisionType::DOG;
	bdog->SetUserData(dogBody);
	//---------------------------------------------------------------------------//

	return true;
}

// Called each loop iteration
bool Enemy::PreUpdate()
{
	/*OPTICK_CATEGORY("lvl1 PreUpdate", Optick::Category::Scene);*/
	return true;
}

// Called each loop iteration
bool Enemy::Update(float dt)
{
	onGround = false;
	if (dogBody->body->GetLinearVelocity().y == 0) onGround = true;

	// Position of enemy is restarted if game is restarted
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		dogBody->body->SetTransform({ PIXEL_TO_METERS(startPosDog.x), PIXEL_TO_METERS(startPosDog.y) }, 0.0f);
	}

	//if (app->map->debugColliders == false)
	//{
	//	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	//	{
	//		app->SaveGameRequest();
	//		F6Load = true;
	//	}
	//}

	//if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//	app->physics->CreateRectangle(250, 100, 32, 32, 0);

	//if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	//{
	//	app->map->debugColliders = !app->map->debugColliders;
	//}


	// update animation

	currentDogAnim->Update();
	currentBirdAnim->Update();
	currentCatAnim->Update();
	currentRatAnim->Update();

	return true;
}

// Called each loop iteration
bool Enemy::PostUpdate()
{

	bool ret = true;


	SDL_Rect rectCat = currentCatAnim->GetCurrentFrame();
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		currentCatAnim = &catsRunL;
		app->render->DrawTexture(cat, 630, 1046, &rectCat);
	}
	else
	{
		app->render->DrawTexture(cat, 630, 1046, &rectCat);
	}

	SDL_Rect rectRat = currentRatAnim->GetCurrentFrame();
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		currentRatAnim = &ratsRunL;
		app->render->DrawTexture(rat, 760, 988, &rectRat);
	}
	else
	{
		app->render->DrawTexture(rat, 760, 988, &rectRat);
	}


	SDL_Rect rectD = currentDogAnim->GetCurrentFrame();
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		currentDogAnim = &dogsDieL;
		app->render->DrawTexture(dog, 1152, 915, &rectD);
	}
	else 
	{
		app->render->DrawTexture(dog, 1152, 915, &rectD);
	}

	SDL_Rect rectB = currentBirdAnim->GetCurrentFrame();
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		currentBirdAnim = &birdFlyL;
		app->render->DrawTexture(bird, 864, 800, &rectB);
	}
	else
	{
		app->render->DrawTexture(bird, 864, 800, &rectB);
	}

	
	return ret;
}


void Enemy::OnCollision(Collider* collider)
{
	/*if (!invincible) health--;
	if (health <= 0)
	{
		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y);
		int random = rand() % 100;

		if (powerUp)
		{
			if (random <= 3)
			{
				App->particles->AddParticle(App->particles->powerUp_Auto, position.x, position.y, Collider::Type::PU_AUTO);
			}
			else if (random >= 50 && random <= 55)
			{
				App->particles->AddParticle(App->particles->powerUp_ThreeWay, position.x, position.y, Collider::Type::PU_THREEWAY);
			}
			else if (random >= 96)
			{
				App->particles->AddParticle(App->particles->powerUp_Pow, position.x, position.y, Collider::Type::PU_POW);
			}
		}
		App->audio->PlayFx(destroyedFx);

		SetToDelete();*/
	/*}*/

}

bool Enemy::CleanUp()
{
	LOG("Destroying Enemy");
	bool ret = true;
	app->tex->UnLoad(texture);
	/*app->physics->world->DestroyBody(b);*/
	return ret;
}