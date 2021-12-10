#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;
struct Collider;

class Enemy : public Module
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Enemy(bool startEnabled);

	// Destructor
	~Enemy();

	// Returns the enemy's collider
	const Collider* GetCollider() const;


	// Collision response
	// Triggers an animation and a sound fx
	virtual void OnCollision(Collider* collider);


	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* dog;
	SDL_Texture* bird;
	SDL_Texture* cat;
	SDL_Texture* rat;


	SString textureDog;
	SString textureBird;
	SString textureCat;
	SString textureRat;

	Animation dogsR;
	Animation dogsL;
	Animation dogsRunR;
	Animation dogsRunL;
	Animation dogsDieR;
	Animation dogsDieL;
	Animation dogsHurtR;
	Animation dogsHurtL;
	Animation dogsBarkR;
	Animation dogsBarkL;
	Animation catsR;
	Animation catsL;
	Animation catsRunR;
	Animation catsRunL;
	Animation catsDieR;
	Animation catsDieL;
	Animation catsHurtR;
	Animation catsHurtL;
	Animation ratsR;
	Animation ratsL;
	Animation ratsRunR;
	Animation ratsRunL;
	Animation ratsDieR;
	Animation ratsDieL;
	Animation ratsHurtR;
	Animation ratsHurtL;
	Animation birdR;
	Animation birdL;
	Animation birdFlyR;
	Animation birdFlyL;
	Animation birdDieR;
	Animation birdDieL;
	Animation birdHurtR;
	Animation birdHurtL;

	Animation* currentDogAnim = nullptr;
	Animation* currentBirdAnim = nullptr;
	Animation* currentCatAnim = nullptr;
	Animation* currentRatAnim = nullptr;


public:
	// The current position in the world
	iPoint position;

	// The enemy's texture
	SDL_Texture* texture = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	int health;
	bool invincible = false;
	bool powerUp = true;
	/*iPoint direction = { 0, 0 };*/
	// Position of the player in the map
	iPoint positionDog;
	iPoint startPosDog;
	b2Vec2* velocity;

	// player's body
	PhysBody* dogBody;
	b2Body* bdog;

	//add a shape
	b2CircleShape dogCircle;

	// The speed in which we move the player (pixels per frame)
	int speed;

	// Jumps of the player
	int jump;
	bool onGround;

protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// The enemy's collider
	Collider* collider = nullptr;

	// Original spawn position. Stored for movement calculations
	/*iPoint spawnPos;*/
};

#endif // __ENEMY_H__