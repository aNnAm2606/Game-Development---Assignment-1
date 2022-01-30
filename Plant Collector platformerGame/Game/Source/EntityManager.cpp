#include "EntityManager.h"
#include "App.h"
#include "Physics.h"
#include "Player.h"
#include "Dog.h"
#include "Cat.h"
#include "Bird.h"
#include "Enemy.h"
#include "Audio.h"
#include "Map.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{
	CleanUp();
}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;
	playerFolder.Create(config.child("playerTexture").child_value());
	dogFolder.Create(config.child("dogTexture").child_value());
	catFolder.Create(config.child("catTexture").child_value());
	birdFolder.Create(config.child("birdTexture").child_value());
	//L13: TODO 6: Initialize Entities from XML 

	return ret;
}

bool EntityManager::Start()
{
	playerTex = app->tex->Load(playerFolder.GetString());
	dogTex = app->tex->Load(dogFolder.GetString());
	catTex = app->tex->Load(catFolder.GetString());
	birdTex = app->tex->Load(birdFolder.GetString());

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint position)
{
	Entity* entity = nullptr; 

	//L13: TODO 1: Create an Entity and add it to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
	{
		entity = new Player(position);
	}break;
	case EntityType::DOG:
	{
		entity = new Dog(position);
	}break;
	case EntityType::CAT:
	{
		entity = new Cat(position);
	}break;
	case EntityType::BIRD:
	{
		entity = new Bird(position);
	}break;
	}

	if (entity != nullptr)
	{ 
		entity->pBody->listener = this;
		entities.add(entity);
	}

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.add(entity);
}

bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		if (ent->data->pendingToDelete)
		{
			DestroyEntity(ent->data);
			break;
		}
	}
	return true;
}

bool EntityManager::Update(float dt)
{
	doLogic = true;

	UpdateAll(dt, doLogic);

	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	if (doLogic)
	{
		for (item = entities.start; item != NULL && ret == true; item = item->next)
		{
			pEntity = item->data;

			if (pEntity->active == false) continue;
			ret = item->data->Update(dt);
		}
	}

	return ret;
}

bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		switch (ent->data->type)
		{
		case EntityType::PLAYER: app->render->DrawTexture(playerTex, ent->data->position.x - 2, ent->data->position.y - 4, &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::DOG: app->render->DrawTexture(dogTex, ent->data->position.x-16, ent->data->position.y -16, &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::CAT: app->render->DrawTexture(catTex, ent->data->position.x - 16, ent->data->position.y - 16, &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::BIRD: app->render->DrawTexture(birdTex, ent->data->position.x - 16, ent->data->position.y - 16, &ent->data->currentAnimation->GetCurrentFrame()); break;
		}
	}
	if (app->map->debugColliders == true)
	{
		app->map->DrawColliders();
	}

	return true;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	bool ret = true;

	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->LoadState(data.child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

bool EntityManager::SaveState(pugi::xml_node& data)
{
	bool ret = true;

	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		//data.append_child(item->data->name.GetString());
		ret = item->data->SaveState(data.child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

void EntityManager::SetPlayer(Player* player_)
{
	player = (Player*)player_;
}

void EntityManager::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::LADDERCOL)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && player->pBody->body->GetLinearVelocity().y < -0.25f)
		{
			player->pBody->body->SetLinearVelocity({ 0,-5.0f });
		}
	}

	if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::STONEWINCOL)
	{
		if (player->chestOpen == true)
		{
			LOG("you won!");
			player->win = true;
		}
	}

	if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::CHESTCOL)
	{
		LOG("open chest!");
		if (player->keyFound == true) 
		{ 
			LOG("it works");
			player->chestOpen = true;
		}
	}

	if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::CHECKPOINT)
	{
		LOG("Checkpoint reached!");
		player->checkPoint = true;
	}

	//ENEMY LOGIC

	//if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::DOGCOL)
	//{
	//	if (player->onGround == true)
	//	{
	//		LOG("THE DOG BIT YOU!");
	//		if (player->GodMode == false && app->entityManager->entities.Find()dog->dogDead == false) player->playerHit = true;
	//	}
	//	else
	//	{
	//		LOG("YOU KILLED THE DOG!");
	//		if (app->enemy->dogDead == false)
	//		{
	//			app->audio->PlayFx(app->enemy->dogSound);
	//			player->pBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
	//		}
	//		app->enemy->dogDead = true;
	//	}
	//}

	//// CAT LOGIC 
	//if (bodyA->colType == CollisionType::PLAYERCOL && bodyB->colType == CollisionType::CATCOL)
	//{
	//	if (player->onGround == true)
	//	{
	//		LOG("THE CAT SCRATCHED YOU!");
	//		if (player->GodMode == false && app->enemy->catDead == false) player->playerHit = true;
	//	}
	//	else
	//	{
	//		LOG("YOU KILLED THE CAT!");
	//		if (app->enemy->catDead == false)
	//		{
	//			app->audio->PlayFx(app->enemy->catSound);
	//			player->pBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
	//		}
	//		app->enemy->catDead = true;
	//	}
	//}

	//// ENEMY LOGIC 
	if (bodyA->entityListener->type == PLAYER && bodyB->listener == this && bodyA->listener == this)
	{
		if (bodyB->entityListener->type == BIRD || bodyB->entityListener->type == DOG)
		{
			if (player->onGround == true)
			{
				LOG("ENEMY KILLED YOU!");
				if (player->GodMode == false && bodyB->listener->active == true) player->playerHit = true;
			}
			else
			{
				LOG("YOU KILLED THE ENEMY!");
				if (bodyB->listener->active == true)
				{
					player->pBody->body->ApplyLinearImpulse({ -0.5f, -2.5f }, { 0,0 }, true);
				}
				bodyB->listener->active = false;
			}
		}
	}

}
