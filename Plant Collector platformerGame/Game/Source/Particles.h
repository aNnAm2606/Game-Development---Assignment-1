#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;
enum Type {
	NOTHING = -1,
	COIN,
	EXPLOSIONS,
	HEARTS,
	KEYS,
	POWERUPS
};

class Particles : public Module
{
public:
	// Constructor
	Particles(bool startEnabled);

	// Destructor
	~Particles();

	// Awake
	bool Awake(pugi::xml_node&);

	// Create Particles
	void CreateParticles(Type type, int x, int y);

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() override;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	// Save and load the position of the playerç
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

private:

	SDL_Texture* coin;
	SDL_Texture* noCoin;
	SDL_Texture* heart;
	SDL_Texture* noHeart;


	SString textureCoin;
	SString textureHeart;

	Animation coins;
	Animation noCoins;
	Animation hearts;
	Animation noHearts;

	Animation* currentCoinsAnim = nullptr;
	Animation* currentHeartsAnim = nullptr;

public:
	iPoint position;
	iPoint startPos;
	b2Vec2* velocity;

	// player's body
	PhysBody* playerBody;
	b2Body* b;

	// Type of particle
	Type type;

	//add a shape
	b2CircleShape playerCircle;

	// The speed in which we move the player (pixels per frame)
	int speed;

	// Jumps of the player
	int jump;
	bool onGround;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Texture* controlsTex;
	SDL_Texture* tutorialsTex;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnimR;
	Animation idleAnimL;
	Animation climbAnim;
	Animation climbDownAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation rightAnimShift;
	Animation leftAnimShift;
	Animation jumpL;
	Animation jumpR;
	Animation fallL;
	Animation fallR;

	// Tutorials animations
	Animation JumpTut;
	Animation ladderTut;
	Animation openTut;

	SDL_Rect tutRect;
	SDL_Rect chestRect;
	SDL_Rect ladderRect;
	SDL_Rect coinRect;
	SDL_Rect heartRect;
	SDL_Rect rect;

	// A flag to detect when the player has been destroyed
	int lives = 3;
	bool controlsVisible = false;
	bool tutorialVisible = false;
	bool chestFound = false;
	bool chestOpen = false;
	bool ladderClose = false;
	bool coinCollision = false;
	bool coinCollected = false;
	bool GodMode = false;
	bool win = false;


	// XML files
	SString playerSprites;
	SString controls;
	SString tutorials;

	// Sound effects indices
	//uint laserFx = 0;
	//uint explosionFx = 0;
	//uint loopFx = 0;
};

#endif // __PARTICLES_H__