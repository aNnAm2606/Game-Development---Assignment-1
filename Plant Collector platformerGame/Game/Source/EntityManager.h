#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"

class Textures;
class Audio;
class Collisions;
class PathFinding;
class Render;
class Player;

class EntityManager : public Module
{
public:

	EntityManager(bool startEnabled);

	// Destructor
	virtual ~EntityManager();

	bool Start();
	// Called before render is available
	bool Awake(pugi::xml_node&);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	// Additional methods
	void SetPlayer(Player* player);

	Entity* CreateEntity(EntityType type, iPoint position);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);

public:

	List<Entity*> entities;
	Player* player = nullptr;
	Input* input;
	Textures* tex;
	Audio* audio;
	Collisions* collisions;
	PathFinding* path;
	Render* render;

	SDL_Texture* playerTex;
	SDL_Texture* dogTex;
	SDL_Texture* catTex;
	SDL_Texture* birdTex;

	SString playerFolder;
	SString dogFolder;
	SString catFolder;
	SString birdFolder;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
