#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"
#include "Physics.h"

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

	// Collision response
	// Triggers an animation and a sound fx
	// Collision callback, called when the player intersects with another collider
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;


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

	// Save and load the position of the enemies
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

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

	b2Vec2 DogVelocity;
	b2Vec2 CatVelocity;
	b2Vec2 RatVelocity;
	b2Vec2 BirdVelocity;

	bool dogLimitR = false;
	bool dogLimitL = true;
	bool catLimitR = false;
	bool catLimitL = true;
	bool birdLimitR = false;
	bool birdLimitL = true;
	bool ratLimitR = false;
	bool ratLimitL = true;


public:

	// The current position in the world
	iPoint dogPosition;
	iPoint catPosition;
	iPoint ratPosition;
	iPoint birdPosition;

	// The enemy's texture
	SDL_Texture* texture = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	int health;
	bool invincible = false;
	bool dogDead = false;
	bool catDead = false;
	bool birdDead = false;

	/*iPoint direction = { 0, 0 };*/
	// Position of the player in the map
	iPoint positionDog;
	iPoint startPosDog;
	iPoint positionCat;
	iPoint startPosCat;
	iPoint positionBird;
	iPoint startPosBird;
	iPoint positionRat;
	iPoint startPosRat;

	b2Vec2* velocity;

	// player's body
	PhysBody* dogBody;
	b2Body* bdog;
	PhysBody* catBody;
	b2Body* bcat;
	PhysBody* birdBody;
	b2Body* bbird;
	PhysBody* ratBody;
	b2Body* brat;

	//add a shape
	b2CircleShape dogCircle;
	b2CircleShape catCircle;
	b2CircleShape ratCircle;
	b2CircleShape birdCircle;

	// The speed in which we move the player (pixels per frame)
	int speed;

	uint catSound;
	uint birdSound;
	uint dogSound;

	bool dontPlayAudioBird = false;
	bool dontPlayAudioCat = false;
	bool dontPlayAudioDog = false;

	bool onGround;
};

#endif // __ENEMY_H__